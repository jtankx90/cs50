from flask_admin.contrib.sqla import ModelView
from wtforms.fields import PasswordField
from flask import redirect,url_for
from webshop import db, login_manager, app,admin, bcrypt,request
from flask_login import UserMixin, current_user
from webshop.models import User,Item,Item_img
from flask_admin.base import AdminIndexView
from flask_admin import Admin,expose
from flask_admin.menu import MenuLink


@login_manager.user_loader
def load_user(user_id):
    return User.query.get(int(user_id))
class AuthMixin(object):

    @app.errorhandler(403) 
    def page_forbiden(e): 
        return redirect(url_for("login"))




class LogoutMenuLink(MenuLink):

    def is_accessible(self):
        return current_user.is_authenticated             


class HomeLink(MenuLink):

    def is_accessible(self):
        return current_user.is_authenticated      


class UserAdmin(AuthMixin,ModelView):
    def is_accessible(self):
        return current_user.is_authenticated and current_user.email == 'admin@gmail.com'
    
    def inaccessible_callback(self, name, **kwargs):
        return redirect(url_for('login'))
    # Don't display the password on the list of Users
    column_exclude_list = ('password_hash')
    form_excluded_columns = ('password_hash')

    def scaffold_form(self):
        form_class = super(UserAdmin, self).scaffold_form()
        form_class.password2 = PasswordField('New Password')
        return form_class
    def on_model_change(self, form, model, is_created):
        if len(model.password2):
                model.password_hash = bcrypt.generate_password_hash(model.password2).decode('utf-8')

class Adminview2(ModelView):
    def is_accessible(self):
        return current_user.is_authenticated and current_user.email == 'admin@gmail.com'
        
    def inaccessible_callback(self, name, **kwargs):
        return redirect(url_for('login'))


class MyAdminIndexView(AuthMixin,AdminIndexView):
    @expose('/<{{url_for(admin.index)}}>')
    def index(self):
        if not current_user.is_authenticated:
            return redirect(url_for('login',next=request.url))
    def is_accessible(self):
        return current_user.is_authenticated and current_user.email == 'admin@gmail.com'
    def inaccessible_callback(self, name, **kwargs):
        return redirect(url_for('login',next=request.url))
   
from webshop.models import MyAdminIndexView



admin = Admin(app,index_view = MyAdminIndexView())
admin.add_view(UserAdmin(User, db.session))
admin.add_view(Adminview2(Item, db.session))
admin.add_view(Adminview2(Item_img, db.session))
admin.add_link(HomeLink(name='Home',category='',url='/'))

admin.add_link(LogoutMenuLink(name='Logout', category='', url="/logout"))

