import os
import csv
from io import StringIO
from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session,make_response
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd


# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True
app.config['SECRET_KEY'] = "pk_c9976d307b964c16a2ab7013015d4255"

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
#if not os.environ.get("API_KEY"):
#    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    rows = db.execute("SELECT cash from users where id = ?",session["user_id"])
    rows2 = db.execute("SELECT symbol,name, shares from (SELECT symbol,name,sum(shares) as shares from index_tbl where user_id = ? group by symbol) where shares >0 ", session["user_id"])
    current_port = {'Symbol':[],'Name':[],'Shares':[],'Price':[],'Total':[]}
    current_headers = [k for k in current_port.keys()]
    current_headers.insert(0,'Action')
    for row2 in rows2:
        current_port["Symbol"].append(row2["symbol"].upper())
        current_port['Name'].append(row2['name'])
        current_port['Shares'].append(row2['shares'])
        price = lookup(row2['symbol'])['price']
        current_port['Price'].append(usd(price))
        current_port['Total'].append(float(row2['shares'] * price))

    current_port["Symbol"].append('CASH')
    current_port['Total'].append(float(rows[0]["cash"]))
    length = len(rows2)+len(rows)

    return render_template("index.html",current_port = current_port, current_headers = current_headers, length = len(rows2) + len(rows))

   # return apology("TODO")


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():

    if request.method == 'POST':
        print('why post')
        symbol = request.form.get('symbol').upper()
        try:
            qty = int(request.form.get('shares'))
        except:
            return apology("Shares field must be integer", 400)

        if lookup(symbol) and isinstance(qty,int):

            if qty > 0:
                buy_price =lookup(symbol)['price']
                name = lookup(symbol)['name']

                try:
                    avail_cash = db.execute("SELECT cash from users where id = ?",session["user_id"])

                    if int(qty)*float(buy_price) <= float(avail_cash[0]['cash']):
                        db.execute("INSERT into Transactions (type,user_id,symbol,quantity,Price) VALUES (?,?,?,?,?)",'BUY',session['user_id'],symbol.upper(),qty,buy_price)
                        cash_left = float(avail_cash[0]['cash']) - int(qty)*float(buy_price)
                        db.execute("UPDATE users set cash = ? where id = ? ", cash_left,session['user_id'])
                        db.execute("INSERT into index_tbl (user_id,Symbol,Name,Shares) VALUES (?,?,?,?)",session['user_id'],symbol,name,int(qty))

                        flash(f'{qty} shares of {name} successfully bought at {buy_price}!' ,'success')
                        return redirect("/")

                    else:
                        flash(f'Not enough cash!' ,'danger')
                        return apology("Not enough cash!", 400)
                except:
                    return apology("Error executing buy trade", 400)
            else:
                return apology("Number of shares must be positive", 400)
        else:
           # if qty <= 0:
            return apology("Invalid Ticker symbol and/or shares field must be integer", 400)

         #   elif not lookup(symbol):
#
           #     return apology("Invalid ticker symbol",400)
    """Buy shares of stock"""
    if request.method == "GET":

        name = request.args.get('type')

    return render_template("buy.html",name = name)


@app.route("/history")
@login_required
def history():
    history_rows = db.execute("SELECT UPPER(symbol) as Symbol,quantity as Shares, Price as Price, datetime as Transacted from transactions where user_id = ? order by Transacted desc",session['user_id'])
    length = len(history_rows)
    headers = ['Symbol','Shares','Price','Transacted']
    return render_template("history.html",rows = history_rows, headers = headers, length = length)

    """Show history of transactions"""
   # return apology("TODO")


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
        # Forget any user_id, but maintain flashed message if present
    if session.get("_flashes"):
        flashes = session.get("_flashes")
        session.clear()
        session["_flashes"] = flashes
    else:
        session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        if request.form['btn'] == 'login':
        # Ensure username was submitted
            if not request.form.get("username"):
                return apology("must provide username", 403)

        # Ensure password was submitted
            elif not request.form.get("password"):
                return apology("must provide password", 403)

        # Query database for username
            rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
            if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
                return apology("invalid username and/or password", 403)

        # Remember which user has logged in
            session["user_id"] = rows[0]["id"]

        # Redirect user to home page
            flash(f'Login succesful for {request.form.get("username")}!','success')
            return redirect("/")
        else:
            return redirect("/reset_pw")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/reset_pw", methods=["GET", "POST"])
def reset_pw():
    if request.method == "POST":
        new_user = request.form.get('username')
        pw = request.form.get('password')
        new_pw = request.form.get('new_pw')
        new_pw2 = request.form.get('new_pw2')


        rows = db.execute("SELECT * from users where username = ?",  request.form.get("username"))


        if len(rows) !=1 or not check_password_hash(rows[0]["hash"], pw):
            return apology("invalid username and/or password", 403)

        else:
            if (new_pw != new_pw2):
                return apology("new password does not match",403)

            else:
                newhashpw = generate_password_hash(new_pw)
                db.execute("UPDATE users set hash = ? where username = ?",newhashpw,new_user)
                flash(f'Password succesfully changed! Please log in again','success')
                return redirect("/login")

    return render_template('reset_pw.html')


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
    if request.method == 'POST':
        symbol = request.form.get('symbol').upper()

        lookup_dict = lookup(symbol)

        if lookup_dict:
            return render_template('quoted.html', lookup_dict = lookup_dict)
        else:
            return apology("Invalid symbol")

    """Get stock quote."""
    return render_template('quote.html')


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        new_user = request.form.get('username')
        pw = request.form.get('password')
        pw2 = request.form.get('confirmation')

        if new_user and pw and pw2 and (pw == pw2):
            check_users = db.execute("SELECT * from users where username = ?", new_user)

            if len(check_users) < 1:
                hash_pw = generate_password_hash(pw)
                db.execute("INSERT into users (username,hash) VALUES (?,?) ", new_user, hash_pw)

                rows = db.execute("SELECT * from users where username = ?", new_user)
                session["user_id"] = rows[0]["id"]
                flash(f'Account created for {new_user}!','success')



                return redirect("/")
            else:
                return apology("Username taken. Please try a different one")


        elif not new_user:
            return apology("Username cannot be empty")
        elif (pw != pw2):
            return apology("Password different. Please check")
        elif (not pw and not pw2):
            return apology("Password fields cannot be empty")
    return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():

    if request.method == "POST":
        symbol = request.form.get('symbol').upper()
        shares = int(request.form.get('shares'))
        sell_price = float(lookup(symbol)['price'])
        name = lookup(symbol)['name']
        rows2 = db.execute("SELECT sum(shares) as shares from index_tbl where user_id = ? and symbol = ? group by symbol", session['user_id'],symbol)
        if len(rows2) == 1:
            if int(rows2[0]['shares']) < int(shares):
                return apology('Not enough shares to sell')
            else:
                try:
                    db.execute("INSERT into index_tbl (user_id,Symbol,Name,Shares) VALUES (?,?,?,?)",session['user_id'],symbol,name,-int(shares))
                    db.execute("INSERT into Transactions (type,user_id,symbol,quantity,Price) VALUES (?,?,?,?,?)",'SELL',session['user_id'],symbol,-shares,sell_price)
                    avail_cash = db.execute("SELECT cash from users where id = ?",session["user_id"])
                    new_cash = (int(shares)* float(sell_price)) + float(avail_cash[0]['cash'])
                    db.execute("UPDATE users set cash = ? where id = ? ", new_cash,session['user_id'])

                    flash(f'{shares} shares of {name} successfully sold at {sell_price}!' ,'success')

                    return redirect("/")
                except:
                    flash(f'Error in selling shares!' ,'danger')
                    return apology("Error in selling shares")
        else:
            return apology('You do not own any shares')
    symbol =[]
    rows2 = db.execute("SELECT symbol from (SELECT symbol,name,sum(shares) as shares from index_tbl where user_id = ? group by symbol) where shares >0 ", session["user_id"])
    for rows in rows2:
        symbol.append(rows['symbol'].upper())


    if request.method == "GET":

        name2 = request.args.get('type')


    return render_template("sell.html", symbol = symbol,name2=name2)

@app.route("/cash", methods=["GET", "POST"])
@login_required
def cash():
    if request.method == 'POST':

        try:
            cash_amount = float(request.form.get('cash_amount'))
        except:
            return apology("Amount to add must be float", 400)

        rows = db.execute("SELECT cash from users where id = ?", session['user_id'])
        exist_cash = float(rows[0]['cash'])
        new_cash_amount = float(cash_amount) + exist_cash

        if new_cash_amount >= 0:

            symbol = "CASH"
            db.execute("UPDATE users set cash = ? where id = ?", new_cash_amount,session['user_id'])

            db.execute("INSERT into Transactions (type,user_id,symbol,quantity,Price) VALUES (?,?,?,?,?)",'ADD',session['user_id'],symbol,cash_amount,1)

            if cash_amount >= 0:
                string2use = 'added'
            else:
                string2use = 'reduced'
                cash_amount *=-1
            flash(f'Cash balance {string2use} by ${cash_amount:,.2f} succesfully' ,'success')
            return redirect("/")
        else:

            flash(f'Unable to reduce cash balance to negative amount. Please enter amount no less than ${exist_cash:,.2f}','danger' )
            return render_template('cash.html')

    return render_template('cash.html')

@app.route("/download", methods=["GET","POST"])
@login_required
def download():
    from flask import Response

    def generate(data):
        si = StringIO()
        cw = csv.writer(si)
        cw.writerow(('Symbol','Shares','Price','Transacted'))
        yield si.getvalue()
        si.seek(0)
        si.truncate(0)
        for item in data:
            new_tuple =[item['Symbol'],item['Shares'],item['Price'],item['Transacted']]
            new_tuple = tuple(new_tuple)

            cw.writerow(new_tuple)
            yield si.getvalue()
            si.seek(0)
            si.truncate(0)

    history_rows = db.execute("SELECT UPPER(symbol) as Symbol,quantity as Shares, Price as Price, datetime as Transacted from transactions where user_id = ?",session['user_id'])
    response = Response(generate(history_rows), mimetype='text/csv')
    response.headers.set("Content-Disposition", "attachment", filename="transactions.csv")
    return response



def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)