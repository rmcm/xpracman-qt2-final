/*
 *     $Id: qmvexception.h 338 2001-04-29 23:37:51Z source $
 * 
 *   Copyright 2001 X=X Computer Software Trust
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

#ifndef QMVEXCEPTION_H
#define QMVEXCEPTION_H

/*
 * ======================================================================
 * DESCRIPTION: Exception Class
 *
 * ======================================================================
 */

/*
 * ======================================================================
 * INCLUDES:
 * ======================================================================
 */
#include <qstring.h>

/*
 * ======================================================================
 * CLASS DEFINITION
 * ======================================================================
 */

class QmvException
{
    
      public:
    enum etype { Information, Warning, Critical };
    QmvException( const QString & msg,  const etype st = Critical );    // Constructor
    ~QmvException();  // Destructor
    const QString & eMessage() const;
    const etype eStatus() const;
          
      private:
    QString message;
    etype status;
          
}; /* CLASS QmvException */

#endif /* QMVEXCEPTION_H */
// Local Variables:
// mode:C++
// End:
