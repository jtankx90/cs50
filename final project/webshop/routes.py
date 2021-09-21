
from webshop import app, db, mail, bcrypt, admin
from flask import render_template, redirect,url_for, flash, get_flashed_messages, request
from webshop.models import Item,User,Item_img
from webshop.forms import RegisterForm, LoginForm, UpdateAccountForm, ResetPasswordForm, RequestResetForm
from flask_login import login_user, current_user, logout_user, login_required
from flask_mail import Message
from flask_admin.contrib.sqla import ModelView

import os
from datetime import datetime, timedelta
from random import random
from sqlalchemy import exc, delete
import json
import os
    
@app.route('/')
@app.route('/home')
def index():
    dt = datetime.today() - timedelta(days=3)
    item= db.session.query(Item,Item_img).filter(Item.sale.is_(False)).join(Item_img,Item.id==Item_img.item_id).limit(3).all()
    new_item  = set(item)
    return render_template('index.html',item=item)

@app.route('/about')
def about():
    return render_template('about.html')

@app.route('/sale')
@login_required
def sale():
    items = Item.query.filter(Item.sale.is_(True)).all()
    img = Item_img.query.all()
    return render_template('sale.html',img=img, items = items)   
 
@app.route('/product')
def product():
    
    items = Item.query.filter(Item.sale.is_(False)).order_by(Item.datetime.desc()).all()
    img = Item_img.query.all()
 

    dt = datetime.today() - timedelta(days=3)

    return render_template('product.html',items = items, img= img,dt=dt.date())

@app.route('/upload', methods=['POST','GET'])
@login_required
def upload():
    def insert(data):
        if data:
            try:
                for images in data['item_img']:
                    img = Item_img(name=images['name'].capitalize(),img_path=images['img_path'])
                    db.session.add(img)
                    db.session.commit()
                for items in data['items']:
                    
                    item = Item(name=items['name'].capitalize(),long_name=items['long_name'], price=int(
                        items['price']), dimension=items['dimension'],description=items['description'], sale=int(items['sale']))
                    db.session.add(item)
                    db.session.commit()
                    imges = Item_img.query.filter_by(name=items['name'].capitalize()).all()
                    for x in imges:
                        x.item_id = Item.query.filter_by(name=items['name'].capitalize()).first().id
                        db.session.add(x)
                    db.session.commit()

                return True

            except exc.IntegrityError:
                db.session.rollback()
                return False

            except Exception as e: 
                print(e)
                db.session.rollback()
                return False
        else:
            return False   
    def delete(data):
        try:
            for dat in data['name']:
                user = db.session.query(Item).filter(Item.name==dat.capitalize()).first()
                db.session.delete(user)
                db.session.commit()
            return True

        except Exception as ex:
            db.session.rollback()
            return False
    insert_boo,delete_boo=False,False
    if request.method == 'POST':
        if 'insertfile' in request.files:
             data = request.files['insertfile']
             insert_data = json.load(data)
             insert_boo = insert(insert_data)
        insert_boo=True
        if 'deletefile' in request.files:
             data = request.files['deletefile']
             delete_data = json.load(data)
             delete_boo = delete(delete_data)
        else:
            delete_boo=True
        if insert_boo and delete_boo:
            flash('Successfully uploaded files','success')
    return redirect(url_for('admin.index'))




@app.route('/register', methods=['GET','POST'])
def register():
    if current_user.is_authenticated:
        return redirect(url_for('index'))
    form = RegisterForm()

    if form.validate_on_submit():
        print('here')
        user_to_create = User(email = form.email.data, password=form.pw1.data,firstname = form.firstname.data, lastname = form.lastname.data)
        db.session.add(user_to_create)
        db.session.commit()
        login_user(user_to_create)
        flash(f'Account created succesfully!','success')

        return redirect(url_for('login'))
    return render_template('register.html',form = form)


@app.route('/login',methods=['GET','POST'])
def login():

    if current_user.is_authenticated:
        return redirect(url_for('index'))

    form = LoginForm()
    if form.validate_on_submit():
        attempted_user = User.query.filter_by(email=form.email.data).first()
        if attempted_user and attempted_user.check_password(
                attempted_password=form.pw1.data
        ):
            login_user(attempted_user)
            flash(f'Welcome {attempted_user.firstname} {attempted_user.lastname}', category='success')
            return redirect(url_for('index'))
        else:
            
            flash('Login Unsuccessful! Please try again', category='danger')

    return render_template('login.html', form=form)

@app.route('/logout')
def logout():
    logout_user()    
    flash("Succesfully log out!", category='info')
    return redirect(url_for("login"))

@app.route('/account',methods=['GET','POST'])
@login_required
def account():
    form = UpdateAccountForm()
    if form.validate_on_submit():
        current_user.email = form.email.data
        db.session.commit()
        flash('account info updated!','success')
        return redirect(url_for('account'))
    elif request.method == 'GET':
        form.email.data = current_user.email
    return render_template('account.html', form=form) 

def send_reset_email(user):
    token = user.get_reset_token()
    msg = Message('Password Reset Request',
                  sender=os.environ.get('EMAIL_USER'),
                  recipients=[user.email])
    msg.body = f'''To reset your password, visit the following link:
{url_for('reset_token', token=token, _external=True)}
If you did not make this request then simply ignore this email and no changes will be made.
'''
    mail.send(msg)

@app.route('/reset_password',methods=['GET','POST'])
def reset_request():
    if current_user.is_authenticated:
        return redirect(url_for('index')) 
    form =RequestResetForm()
    if form.validate_on_submit():
        user = User.query.filter_by(email= form.email.data).first()
        send_reset_email(user)
        flash('An email has been set. Please check your inbox','success')
        return redirect(url_for('login'))
    return render_template('reset_request.html', form=form) 


@app.route('/reset_password/<token>',methods=['GET','POST'])
def reset_token(token):
    if current_user.is_authenticated:
        return redirect(url_for('index')) 
    user = User.verify_reset_token(token)
    if user is None:
        flash('Invalid or expired taken','warning')
        return redirect(url_for('reset_request'))
    form = ResetPasswordForm()
    if form.validate_on_submit():
        hashed_password = bcrypt.generate_password_hash(form.pw1.data).decode('utf-8')
        user.password_hash = hashed_password
        db.session.commit()
        flash('Your password has been updated! You are now able to log in', 'success')
        return redirect(url_for('login'))
    return render_template('reset_token.html',form=form)
