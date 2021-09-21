from webshop import db
from webshop.models import Item,Item_img
from sqlalchemy import exc, delete
import json
import os

filename= r'C:\Users\jtank\Desktop\cs50 scripts\cs50\final project\webshop\static\add_items.json'
def insert_items(filename):
    if os.stat(filename).st_size != 0:
        with open(filename, 'r', encoding="utf-8") as add_file:
            data = json.load(add_file)

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
        except exc.IntegrityError:
            db.session.rollback()
            print('Items already exist')

        except Exception as e: 
            print(e)
            db.session.rollback()   



""" def delete_items(filename):
    if os.stat(filename).st_size != 0:
        with open(filename, 'r') as delete_file:
            data_delete = json.load(delete_file)

        try:
            for dat in data_delete['name']:
                sql1 = delete(Item).where(Item.name == dat.capitalize())
                db.session.execute(sql1)
                db.session.commit()
                
            with open(filename,'w') as delete_file:
                del data_delete['name']
                json.dumps(data_delete)

        except:
            db.session.rollback()
            print(f'Error deleting Items') """



