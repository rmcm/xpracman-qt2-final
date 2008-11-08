/*
 *     $Id: marvin-test.cpp 160 2000-10-26 05:07:27Z source $
 * 
 *   Copyright 2000 X=X Computer Software Trust
 *    		  Kangaroo Ground Australia 3097
 * 
 * 
 *  This is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *  
 *  This software is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License
 *  along with this software; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 *  
 *  Report problems and direct all questions to:
 *  
 *      Rex McMaster, rmcm@compsoft.com.au
 *  
 * 

 */

/*
 * ======================================================================
 * DESCRIPTION: A test main() for marvin interface
 *
 * ======================================================================
 */



/*
 * ======================================================================
 * INCLUDES:
 * ======================================================================
 */
#include <iostream.h>
#include <stdio.h>
#include "marvinclass.h"

/*
 * ======================================================================
 * DEFINES:
 * ======================================================================
 */

/*
 * ======================================================================
 * LOCAL VARIABLES:
 * ======================================================================
 */

/*
 * ======================================================================
 * MEMBER FUNCTIONS:
 * ======================================================================
 */


int main( int argc, char **argv )
{

    
  QmvDB *db = new QmvDB("micr", "Microbiology");
  db->db_classes.insert("aawl", new QmvClass( db, "aawl"));
  db->db_classes.insert("aasp", new QmvClass( db, "aasp"));
  db->db_classes.insert("aarr", new QmvClass( db, "aarr"));
  db->db_classes.insert("patn", new QmvClass( db, "patn"));
  QString * i;
  QmvClass *c;
  QmvAttribute *m_att;

  /*
   * This section displays the loaded meta-meta data
   */
    
  //      for (i = db->att_meta_class.first(); i != 0; i = db->att_meta_class.next())
  //          cout << i << "-> " << *i << endl;
  
  //      for (i = db->att_meta_attribute.first(); i != 0; i = db->att_meta_attribute.next())
  //          cout << i << "-> " << *i << endl;

  //      for (i = db->att_meta_foreign.first(); i != 0; i = db->att_meta_foreign.next())
  //          cout << i << "-> " << *i << endl;

  //      for (i = db->att_meta_relation.first(); i != 0; i = db->att_meta_relation.next())
  //          cout << i << "-> " << *i << endl;

  /*
   * This section displays the metadata by class
   */
    
  //      for (c = db->db_classes.first(); c != 0; c = db->db_classes.next())
  //      {
  //          cout << c->className() << " ... " << c->class_metadata.count() << endl;
  //          QDictIterator<QString> it_class( c->class_metadata  );
  //          while ( it_class.current() ) {
  //              cout << it_class.currentKey().latin1() << "-->" << *(it_class.current())  << endl;
  //              ++it_class;
  //          }
  //          cout << "ATTRIBUTES" << endl;
  //          QDictIterator<QmvAttribute> it_attribute(c->attribute_metadata);
  //          while (it_attribute.current())
  //          {
  //              m_att = it_attribute.current();
  //              QDictIterator<QString> it_attatt(*m_att);
  //              while(it_attatt.current())
  //              {
  //                  cout << it_attribute.currentKey().latin1() << "-->" 
  //                       << it_attatt.currentKey().latin1() << "-->" << *(it_attatt.current()) << endl;
  //                  ++it_attatt;
  //              }
  //              ++it_attribute;
  //          }
  //          cout << "FOREIGNS" << endl;
  //          QDictIterator<QmvAttribute> it_foreign(c->foreign_metadata);
  //          while (it_foreign.current())
  //          {
  //              m_att = it_foreign.current();
  //              QDictIterator<QString> it_attatt(*m_att);
  //              while(it_attatt.current())
  //              {
  //                  cout << it_foreign.currentKey().latin1() << "-->" 
  //                       << it_attatt.currentKey().latin1() << "-->" << *(it_attatt.current()) << endl;
  //                  ++it_attatt;
  //              }
  //              ++it_foreign;
  //          }
  //          cout << "RELATIONS" << endl;
  //          QDictIterator<QmvAttribute> it_relation(c->relation_metadata);
  //          while (it_relation.current())
  //          {
  //              m_att = it_relation.current();
  //              QDictIterator<QString> it_attatt(*m_att);
  //              while(it_attatt.current())
  //              {
  //                  cout << it_relation.currentKey().latin1() << "-->" 
  //                       << it_attatt.currentKey().latin1() << "-->" << *(it_attatt.current()) << endl;
  //                  ++it_attatt;
  //              }
  //              ++it_relation;
  //          }
    
  //      }

  /*
   *  This section displays a specified meta data attribute
   */

//    if (argc > 2)
//      {
//        const char *xclass = argv[1];
//        const char *xattribute = argv[2];
//        const char *xattatt = argv[3];

//        cout << "Testing .. looking for attribute_title for " << xclass << " .. "
//             << " Class: " << db->db_classes[xclass]->className()
//             << " Attribute: " << *(db->db_classes[xclass]->attribute_metadata[xattribute]->find(xattatt)) << endl;
//      } else {
//        cout << "usage: " << argv[0] << " class attribute att-att" << endl;
//      }
    QString *query = new QString ("select * from aawl where 1=1;");
    db->db_classes["aawl"]->open( query );
    //    cout << "Loaded dataset size: " << db->db_classes["aawl"]->loaded_tupples.count() << endl;
  
    //  db->db_classes["aawl"]->print();
    db->db_classes["aawl"]->print(new QString(argv[2]));
    db->db_classes["aawl"]->update(atoi(argv[1]), new QString(argv[2]), new QString(argv[3]));
    db->db_classes["aawl"]->print(new QString(argv[2]));
    db->db_classes["aawl"]->save(atoi(argv[1]));
}

