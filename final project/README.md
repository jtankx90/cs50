# Website for online store
###


###

Hello! Welcome to a sample webstore where you can showcase your items you are looking to sell, and also a dedicated page for sale items reserved exclusively
for registered account users!

Upon arriving the home page, visitors are greeted with a carousel of images of the items that are not on sale - sale items are only for registered users.  

Files used for home page: run.py, __init__.py, routes.py, layout.html, index.html,

## Visitors  

Besides the home page, there are 3 active nav items in the nav bar for visitors:

1. **Product**  
In the product page, all non-sale products are listed here with their information such as price, quantity. There is also a button that provides further information
such as description, dimension and also a carousel of images for the particular product. Visitors are also made aware of items that are newly added into the store with a prompt on the right side
of the item's name.  

    Files used: product.html, items_modal.html

2. **Login**  
In the login page, visitors can login to their registered account via the email and password used during registration. Forgot password functionality is also available for users that have forgotten and wish to reset their password. For the reset of password, users will receive an email sent to their registered email and will have to follow the instructions in the email to reset their password. Take note that there is a time expiration for each requested link.  

    Files used: Login.html, reset_request.html, reset_token.html

3. **Register**  
Vistors can register an account to gain access to sale items. Vistors are required to key in their first and last name, email address, password and also a confirmation password. In the password field, a prompt of messages (requirements) is shown for the specification of a strong password. Each message will turn green indicating the meeting of that requirement. 

    Files used: Register.html

## Registered Users  

Once visitors have logged onto their account, two addiitional nav items will be shown.  

1. **Sale**  
Items that are on sale will be shown in this page in the format of cards in a deck. For each card, similar to the product page, users can click on the more info button that view further information for each of the sale product.  

    Files used: Sale.html, items_modal.html

2. **Account**  
Added option allowing users to change their email address registered with the website.

Files used: account.html

## Admin Account  

This app has an added administrator functionality, for which the designated admin account can access the backend database without the use of any database software. The Admin tab in the nav bar will only be available to the admin account. Any attempts for direct access to the pages will be redirectd to the home page. In the admin page, the admin can do the following:   
* insert and delete entries in the items and item_img table through the upload of a json file for each function;
* Alternatively, each table in the database is presented as a tab, in which the data for each table will be shown. In the user table, the password column has been remove for viewing i.e. even when using the admin account, the hashed password will not be shown. When inserting new users directly (without registering an account), a password input box will be available for inputing the password. The password will then be converted into into it's hashed equivalent and stored in the database

Files used: admin.py, admin/index.html  



### Remaining files  

1) **scrap_reddit_img.py**  
script for scrapping reddit for sample images  

2)  **forms.py**  
contains all the forms used in the app  

3) **models.py**   
data models used in the backend database. consisting of 3 models: User, Items, Item_img  

4) **shop.db**   
sqlite database   
