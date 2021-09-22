
from flask_admin.base import AdminIndexView
from sqlalchemy.orm import backref
from webshop import db, login_manager, app, admin
from flask_login import UserMixin, current_user
from webshop import bcrypt
from itsdangerous import TimedJSONWebSignatureSerializer as Serializer
import datetime
from flask_admin.contrib.sqla import ModelView
from wtforms.fields import PasswordField
from flask import redirect,url_for


@login_manager.user_loader
def load_user(user_id):
    return User.query.get(int(user_id))

class Item(db.Model):
    id = db.Column(db.Integer(),primary_key=True)
    name = db.Column(db.String(length=100), nullable=False,unique=True)
    long_name = db.Column(db.String(length=100),nullable=False)
    price = db.Column(db.Integer(),nullable=False)
    description = db.Column(db.String())
    sale = db.Column(db.Boolean(),default=False)
    datetime= db.Column(db.Date(),default=datetime.datetime.now())
    dimension = db.Column(db.String(),nullable=False)
    item_img = db.relationship('Item_img',backref='img_paths',lazy=True,cascade = "all,delete")
    sale_price = db.Column(db.Integer())
    Quantity = db.Column(db.Boolean(),default=True)
    
    def __repr__(self):
        return f'Item({self.name})'


class Item_img(db.Model):
    id = db.Column(db.Integer(),primary_key=True)
    name = db.Column(db.String(length=100),nullable=False)
    item_id = db.Column(db.Integer(),db.ForeignKey('item.id'))
    img_path = db.Column(db.String(),nullable=False)


class User(db.Model,UserMixin):
    id = db.Column(db.Integer(),primary_key=True)
    firstname = db.Column(db.String(),nullable =False)
    lastname = db.Column(db.String(),nullable=False)
    email = db.Column(db.String(length=120),unique=True,nullable = False)
    password_hash = db.Column(db.String(length =60),nullable=False)


    @property
    def password(self):
        return self.password

    @password.setter
    def password(self, plain_text_password):
        self.password_hash = bcrypt.generate_password_hash(plain_text_password).decode('utf-8')
    
    def check_password(self,attempted_password):
        return bcrypt.check_password_hash(self.password_hash,attempted_password)

    def get_reset_token(self,expires_sec=300):
        s = Serializer(app.config['SECRET_KEY'],expires_sec)
        return s.dumps({'user_id': self.id}).decode('utf-8')

    @staticmethod
    def verify_reset_token(token):
        s = Serializer(app.config['SECRET_KEY']) 
        try:
           user_id = s.loads(token)['user_id']
        except:
            return None
        return User.query.get(user_id)
