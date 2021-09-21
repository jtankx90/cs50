
from flask_wtf import FlaskForm
from flask_login import current_user
from wtforms import StringField, PasswordField, SubmitField
from wtforms.validators import Length, EqualTo, Email, DataRequired, ValidationError
from webshop.models import User

class RegisterForm(FlaskForm):


   def validate_email(self,email_to_check):
      email_add = User.query.filter_by(email = email_to_check.data).first()
      
      if email_add:
         raise ValidationError('Account already exist! Please log in or reset password')
   email = StringField(validators=[Email(),DataRequired()])
   firstname = StringField(validators=[Length(min=1),DataRequired()])
   lastname = StringField(validators=[Length(min=1),DataRequired()])
   pw1 = PasswordField(validators=[Length(min=8),DataRequired()])
   pw2 = PasswordField(validators=[EqualTo('pw1'), DataRequired()])
   submit = SubmitField(label='Register')


class LoginForm(FlaskForm):
    email = StringField(label='Email:   ',validators=[Email(),DataRequired()])
    pw1 = PasswordField(label='Password:', validators=[DataRequired()])
    submit = SubmitField(label='Sign in')

class UpdateAccountForm(FlaskForm):
    email = StringField(validators=[DataRequired(), Email()])
    submit = SubmitField('Update')


    def validate_email(self, email):
        if email.data != current_user.email:
            user = User.query.filter_by(email=email.data).first()
            if user:
                raise ValidationError('That email is taken. Please choose a different one.')

        if email.data == current_user.email:
            raise ValidationError('Identical email address')


class RequestResetForm(FlaskForm):
      email = StringField(validators=[DataRequired(), Email()])
      submit = SubmitField('Request Reset')

      def validate_email(self, email):
            user = User.query.filter_by(email=email.data).first()
            if user is None:
                raise ValidationError('There is no account associated with that email. Please register first')

class ResetPasswordForm(FlaskForm):
      pw1 = PasswordField(validators=[Length(min=8),DataRequired()])
      pw2 = PasswordField(validators=[EqualTo('pw1'), DataRequired()])
      submit = SubmitField('Reset Password')
