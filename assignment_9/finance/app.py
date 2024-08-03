import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    net_worth = 0
    symbols = db.execute(
        "SELECT DISTINCT symbol, SUM(shares) FROM stocks WHERE user_id=? GROUP BY symbol", session["user_id"])
    for i in range(len(symbols)):
        symbols[i]["current_price"] = lookup(symbols[i]["symbol"])["price"]
        symbols[i]["total"] = symbols[i]["current_price"]*symbols[i]["SUM(shares)"]
        net_worth += symbols[i]["total"]

    cash = db.execute("SELECT cash FROM users WHERE id=?", session["user_id"])[0]["cash"]
    net_worth += cash
    return render_template("index.html", symbols=symbols, cash=cash, tot=net_worth)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        if not request.form.get("symbol"):
            return apology("missing symbol", 400)

        elif lookup(request.form.get("symbol")) == None:
            return apology("invalid symbol", 400)

        elif not request.form.get("shares").isdigit() or int(request.form.get("shares")) <= 0:
            return apology("Invalid number of shares", 400)

        symbol = lookup(request.form.get("symbol"))["symbol"]
        price = int(lookup(request.form.get("symbol"))["price"])*int(request.form.get("shares"))
        cash_before = db.execute("SELECT cash FROM users WHERE id=?", session["user_id"])[0]["cash"]
        if price > cash_before:
            return apology("insufficient funds", 400)

        db.execute("UPDATE users SET cash=? WHERE id=?", cash_before-price, session["user_id"])
        db.execute("INSERT INTO stocks (user_id,symbol,shares,price,datetime) VALUES(?,?,?,?,datetime())",
                   session["user_id"], symbol, request.form.get("shares"), price)
        return redirect("/")

    return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    sym = db.execute("SELECT * FROM stocks WHERE user_id=?", session["user_id"])
    return render_template("history.html", sym=sym)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        if not request.form.get("symbol"):
            return apology("missing symbol", 400)
        elif lookup(request.form.get("symbol")) == None:
            return apology("invalid symbol", 400)
        symbol = lookup(request.form.get("symbol"))["symbol"]
        price = usd(lookup(request.form.get("symbol"))["price"])
        return render_template("quoted.html", symbol=symbol, price=price)

    return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    session.clear()

    if request.method == "POST":
        if not request.form.get("username"):
            return apology("must provide username", 400)

        elif not request.form.get("password"):
            return apology("must provide password", 400)

        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("passwords don't match", 400)

        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        if len(rows) != 0:
            return apology("username already exists", 400)

        db.execute("INSERT INTO users (username, hash) VALUES(?,?)", request.form.get(
            "username"), generate_password_hash(request.form.get("password")))

        session["user_id"] = db.execute(
            "SELECT id FROM users WHERE username=?", request.form.get("username"))[0]["id"]
        return redirect("/")

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    symbols = db.execute("SELECT DISTINCT symbol FROM stocks WHERE user_id=?", session["user_id"])
    if request.method == "POST":
        if request.form.get("symbol") not in [symbols[i]["symbol"]for i in range(len(symbols))]:
            return apology("Invalid symbol", 400)

        elif not request.form.get("shares"):
            return apology("missing shares", 400)
        symbol = request.form.get("symbol")
        selling_shares = int(request.form.get("shares"))
        price = int(lookup(request.form.get("symbol"))["price"])*int(request.form.get("shares"))
        existing_shares = int(db.execute(
            "SELECT SUM(shares) FROM stocks WHERE user_id=? AND symbol=?", session["user_id"], symbol)[0]["SUM(shares)"])
        if selling_shares > existing_shares:
            return apology("Insufficient shares", 400)
        db.execute("INSERT INTO stocks (user_id,symbol,shares,price,datetime) VALUES(?,?,?,?,datetime())",
                   session["user_id"], symbol, -selling_shares, price)
        cash_before = db.execute("SELECT cash FROM users WHERE id=?", session["user_id"])[0]["cash"]
        db.execute("UPDATE users SET cash=? WHERE id=?", cash_before+price, session["user_id"])
        return redirect("/")
    return render_template("sell.html", symbols=symbols)


@app.route("/cash", methods=["GET", "POST"])
@login_required
def add_cash():
    if request.method == "POST":
        if not request.form.get("amount").isdigit() or int(request.form.get("amount")) <= 0:
            return apology("Invalid amount", 400)

        cash_before = db.execute("SELECT cash FROM users WHERE id=?", session["user_id"])[0]["cash"]
        db.execute("UPDATE users SET cash=? WHERE id=?", cash_before +
                   int(request.form.get("amount")), session["user_id"])
        return redirect("/")
    return render_template("cash.html")


''' created database
CREATE TABLE stocks (
    id INTEGER NOT NULL,
    user_id INTEGER NOT NULL,
    symbol TEXT NOT NULL,
    shares INTEGER,
    price NUMERIC,
    datetime NUMERIC,
    PRIMARY KEY(id),
    FOREIGN KEY(user_id) REFERENCES users(id)
);
'''
