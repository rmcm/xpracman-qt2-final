/*
 *     $Id: qmvattlist.h 313 2001-04-10 00:59:42Z source $
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

#ifndef QMVATTLIST_H
#define QMVATTLIST_H

/*
 * ======================================================================
 * DESCRIPTION: Reimplementation of QList (ref to QStrList for more
 *              detailed example.
 * ======================================================================
 */

/*
 * ======================================================================
 * INCLUDES:
 * ======================================================================
 */
#include <qstring.h>
#include <qlist.h>
#include <qdatastream.h>
#include "qmvattribute.h"

/*
 * ======================================================================
 * CLASS DEFINITION
 * ======================================================================
 */

typedef QList<QmvAttribute>		QmvAttListBase;

class QmvAttList : public QmvAttListBase
{
    
  public:
    enum cfDataTypes { Unknown, Integer, Numeric, Double, String };
    
    QmvAttList();
    QmvAttList( QString const & index, cfDataTypes type );
    ~QmvAttList();
    QString cf_index;
    cfDataTypes cf_type;
    void print();
    
  private:
    int compareItems( QCollection::Item a1, QCollection::Item a2 );
    
}; /* CLASS QmvAttList */

#endif /* QMVATTLIST_H */
// Local Variables:
// mode:C++
// End:
