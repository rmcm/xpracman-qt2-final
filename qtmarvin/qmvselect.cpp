/*
 *     qmvselect.cpp,v 1.16 2005/03/15 01:10:46 source Exp
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

/*
 * ======================================================================
 * DESCRIPTION: 
 *
 * ======================================================================
 */



/*
 * ======================================================================
 * INCLUDES:
 * ======================================================================
 */

#include <math.h>
#include "qmvselect.h"

/*
 * ======================================================================
 * DEFINES:
 * ======================================================================
 */

static char qmvselect_version[] = "qmvselect.cpp,v 1.16 2005/03/15 01:10:46 source Exp";

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

// Constructor

// --------------------------------------------------------------------------------
QmvSelect::QmvSelect( const QStringList & options, QWidget * parent, const char * name )
        : QDialog( parent, name, TRUE, 0 ),
          current_item(-1), index_column(-1), num_rows(0), num_cols(0), sort_by(0),
          list_view(0), relation(0)
{
        // single column of options
    label_list.append("");
        // load the value list
    for (int i = 0; i < options.count(); i++)
        value_list.append( new QStringList(options[i]) );
        
    init();
}

// --------------------------------------------------------------------------------
QmvSelect::QmvSelect( const QList<QStringList> & values, const QStringList & labels, int sortby,
                      QWidget * parent, const char * name )
        : QDialog( parent, name, TRUE, 0 ),
          current_item(-1), index_column(-1), num_rows(0), num_cols(0), sort_by(sortby),
          value_list(values), label_list(labels), list_view(0), relation(0)
{
        // labels and values are loaded in the arg list
    init();
}


// --------------------------------------------------------------------------------
QmvSelect::QmvSelect( const QList< QList<QPixmap> > & pixmaps, const QList<QStringList> & values,
                      const QStringList & labels, int sortby,
                      QWidget * parent, const char * name )
        : QDialog( parent, name, TRUE, 0 ),
          current_item(-1), index_column(-1), num_rows(0), num_cols(0), sort_by(sortby),
          pixmap_list(pixmaps), value_list(values), label_list(labels), list_view(0), relation(0)
{
        // labels and values are loaded in the arg list
    init();
}

// --------------------------------------------------------------------------------
QmvSelect::QmvSelect( QmvClass * reln, QWidget * parent, const char * name )
        : QDialog( parent, name, TRUE, 0 ),
          current_item(-1), index_column(-1), num_rows(0), num_cols(0), sort_by(0),
          list_view(0), relation(reln)
{    
    if ( relation )
    {
            // load column labels
        const QmvAttribute * att;
        att = relation->attribute( relation->userLabel() );
            // blank column if no attribute matches
        if ( !att )
            label_list += QString("");
        else            
            label_list += att->attributeTitle();
            // load values
        num_rows = relation->count();
        for ( int row = 0; row < num_rows; row++)
        {
            QStringList * sl = new QStringList();
            value_list.append(sl);
            (*sl) += relation->attributeValue(row, relation->userLabel() );
        }
    }
    
    init();
}

// --------------------------------------------------------------------------------
QmvSelect::QmvSelect( QmvClass * reln, QStringList & att_list, QWidget * parent, const char * name )
        : QDialog( parent, name, TRUE, 0 ),
          current_item(-1), index_column(-1), num_rows(0), num_cols(0), sort_by(0),
          list_view(0), relation(reln)
{
    if ( relation )
    {
            // load column labels
        num_cols = att_list.count();
        for (int col = 0; col < num_cols; col++ )
        {
            const QmvAttribute * att;
            att = relation->attribute( att_list[col] );
                // blank column if no attribute matches
            if ( !att )
                label_list += QString("");
            else            
                label_list += att->attributeTitle();
        }
            // load values
        num_rows = relation->count();
        for ( int row = 0; row < num_rows; row++)
        {
            QStringList * sl = new QStringList();
            value_list.append(sl);
            for ( int col = 0; col < num_cols; col++ )
                (*sl) += relation->attributeValue(row, att_list[col]);
        }
    }
    
    init();
}

// Destructor

// --------------------------------------------------------------------------------
QmvSelect::~QmvSelect()
{
    value_list.setAutoDelete(true);
    value_list.clear();
    pixmap_list.setAutoDelete(true);
    pixmap_list.clear();
}

// --------------------------------------------------------------------------------
void QmvSelect::init()
{
    current_item = -1;
    setCaption( tr( "Select" ) );

        // ------------------------------------------------------------
        // Top level layout - add a groupbox, and manage it manually
        // ------------------------------------------------------------
    QVBoxLayout * vl_top = new QVBoxLayout( this );
    QLabel * lbl = new QLabel( "Select from list", this, "selection list" );
    vl_top->addWidget( lbl );

        // ------------------------------------------------------------
        // Create the listview.
        // ------------------------------------------------------------
    list_view = new QListView( this, "select list" );
    vl_top->addWidget( list_view );
    list_view->setAllColumnsShowFocus(TRUE);
    bool increasing = (sort_by >= 0) ? true : false;
    if ( sort_by < 0 ) sort_by = -sort_by;
    list_view->setSorting( sort_by, increasing );
    list_view->setShowSortIndicator( true );

        // clicked
    connect( list_view, SIGNAL( currentChanged( QListViewItem * ) ),
             this, SLOT( highlighted( QListViewItem * ) ) );
        // select
    connect( list_view, SIGNAL( doubleClicked( QListViewItem * ) ),
             this, SLOT( selected( QListViewItem * ) ) );

        // ----------------
        // Column Headings:
        // ----------------
    num_cols = label_list.count();
    for ( int col = 0; col < num_cols; col++ )
        list_view->addColumn( label_list[col] );
    
        // Add the index column at the right
    index_column = list_view->addColumn( "#" );

        // ----------------
        // Load the rows.
        // ----------------
    
    list_view->clear();
    list_view->setFocusPolicy(StrongFocus);
    QListViewItem * item = 0;
    QListViewItem * after = 0;

        // load the rows
    num_rows = value_list.count();
    for ( int row = 0; row < num_rows; row++)
    {
            // Place first item at top of list.
        if ( row == 0 )
            item = new QListViewItem( list_view );
        else
            item = new QListViewItem( list_view, after );

            // Extract the stringlist first
        QStringList * sl = value_list.at(row);
        if (sl) {
                // Load the strings into items
            for ( int col = 0; col < num_cols; col++ ) {
                if (sl->at(col) == sl->end())   // at() returns iterator
                    continue;
                item->setText( col, (*(sl->at(col))) );
            }
        }
            // Extract the pixmap list
        QList<QPixmap> * pl = pixmap_list.at(row);
        if (pl) {
                // Load the pixmaps
            int pm_count = pl->count();
            for ( int col = 0; col < num_cols; col++ ) {
                QPixmap * pm;
                pm = pl->at(col);               // at() returns pointer to pixmap
                if (!pm || pm->isNull() )
                    continue;
                item->setPixmap( col, *pm );
                item->setText( col, "");        // unset the text
            }
            
        }

            // the index column
        int index = 1 + static_cast<int> (floor(log10(num_rows)));
        QString format = QString("0%1d").arg(index);
        format.prepend("%");
        item->setText( num_cols, QString("%1").sprintf(format,row) );

        after = item;
        
    }
        
    QHBoxLayout * hl_button = new QHBoxLayout( vl_top );
    QPushButton *pb;
    pb = new QPushButton("OK", this );
    pb->setDefault(TRUE);
    hl_button->addWidget( pb );
    connect( pb, SIGNAL( clicked() ), this, SLOT( accept() ) );
    pb = new QPushButton( "Cancel", this );
    hl_button->addWidget( pb );
    connect( pb, SIGNAL( clicked() ), this, SLOT( reject() ) );

        // cancel
    connect( this, SIGNAL( escapePressed() ),
             this, SLOT( cancelSelect() ) );

        // Highlight the first row
    
    QListViewItem *first = list_view->firstChild();
    if ( first )
    {
        list_view->setCurrentItem( first );
        list_view->setSelected( first, TRUE );
    } else {
        pb->setFocus();
    }
    

    setFocusProxy( list_view );
    adjustSize();
    updateGeometry();
//  no need to set focus - has been set above by setCurrentItem
//  setFocus();

}

// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
int QmvSelect::setCurrentRow( int row )
{
    int set = -1;
    
    QListViewItem *item = list_view->firstChild();
    while (item)
    {
        item = item->nextSibling();
        if ( !item || item->text(index_column).toInt() != row )
            continue;
        list_view->setCurrentItem( item );
        list_view->setSelected( item, TRUE );
        list_view->ensureItemVisible( item );
        set = row;
        break;
    }
    return set;
}


// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void QmvSelect::highlighted( QListViewItem * item )
{
    int index = item->text(index_column).toInt();
 
    if ( index >= 0 && index < num_rows )
        current_item = index;
}

// --------------------------------------------------------------------------------
void QmvSelect::selected( QListViewItem * item )
{
        // use highlighted to avoid repetition
    highlighted( item );
    done( current_item );
}

// --------------------------------------------------------------------------------
void QmvSelect::cancelSelect(void)
{
    done(-1);
}

// --------------------------------------------------------------------------------
void QmvSelect::accept()
{
    done( current_item );
}

// --------------------------------------------------------------------------------
void QmvSelect::reject()
{
    done( -1 );
}

// --------------------------------------------------------------------------------
void QmvSelect::keyPressEvent( QKeyEvent *ev )
{
    if (ev->key() == Key_Escape)
        emit escapePressed();
    else
        QDialog::keyPressEvent(ev);
    ev->accept();
}
