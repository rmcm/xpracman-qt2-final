/*
 *     $Id: qmvattlist.cpp 1566 2003-02-22 05:33:20Z source $
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
 * DESCRIPTION: A derived version of QList
 *
 * ======================================================================
 */



/*
 * ======================================================================
 * INCLUDES:
 * ======================================================================
 */

#include <stdlib.h>
#include <iostream.h>
#include "qmvattlist.h"

/*
 * ======================================================================
 * DEFINES:
 * ======================================================================
 */

static char qmvattlist_version[] = "$Id: qmvattlist.cpp 1566 2003-02-22 05:33:20Z source $";

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

QmvAttList::QmvAttList()
{
        // Default to view order numeric sort 
    cf_index = QString("mtat_view_order");
    cf_type = Numeric;
}

QmvAttList::QmvAttList( QString const & index, cfDataTypes type )
{
    cf_index = index;
    cf_type = type;
}

QmvAttList::~QmvAttList()
{
     clear();
}


int QmvAttList::compareItems( QCollection::Item a1, QCollection::Item a2 )
{
        // items (ie qmvattribute) == qdict<string>

    int i = 0;

    QString str1;
    QString str2;
    QString * p_str;
    p_str = ((QmvAttribute *) a1)->find(cf_index);
    if ( p_str )
        str1 = *p_str;
    p_str = ((QmvAttribute *) a2)->find(cf_index);
    if ( p_str )
        str2 = *p_str;
    
    if (str1.length() && str2.length())
        switch (cf_type)
        {
            case String:
                i = str1.compare(str2);
                break;
            case Integer:
                i = str1.toInt() - str2.toInt();
                break;
            case Double:
            case Numeric:
                i = int(str1.toDouble() - str2.toDouble());
                break;
            default:
                i = 0;
        }
    else
        i = ((str1)?1:0) - ((str2)?1:0);
        
        // cout << "i=" << i << ":: str1=" << str1.latin1() << "::str2=" << str2.latin1() << endl;
    
    return(i);    
}

void QmvAttList::print()
{
    int i = 0;
    for ( QmvAttribute * at_ptr = first(); at_ptr != 0; at_ptr = next())
        qDebug("QmvAttList::print():%d. %s", i++, at_ptr->find( "mtat_title")->latin1() );
}
