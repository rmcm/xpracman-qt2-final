/*
 *     $Id: qmvquerywidget.cpp 3211 2006-12-04 00:53:56Z source $
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
#include "qmvquerywidget.h"

namespace QmvQueryWidgetIcons
{
#include "find.xpm"
} // end QmvQueryWidgetIcons

/*
 * ======================================================================
 * DEFINES:
 * ======================================================================
 */
 static char qmvquerywidget_version[] = "$Id: qmvquerywidget.cpp 3211 2006-12-04 00:53:56Z source $";

/*
 * ======================================================================
 * LOCAL VARIABLES:
 * ======================================================================
 */

/*
 * ======================================================================
 * MEMBER FUNCTIONS: QmvQueryWidget Class
 * ======================================================================
 */




//--------------------------------------------------------------------------------
//! Constructor - from \ref QmvSqlQuery
/*!
  Construct using an existing \ref QmvSqlQuery object

  \param qryobj 
  \param parent 
  \param name   
*/
//--------------------------------------------------------------------------------
QmvQueryWidget::QmvQueryWidget( QmvSqlQuery * qryobj, QWidget * parent, const char * name)
        : QFrame( parent, name), new_query_object(false),  query_object(qryobj)
{
    connect( query_object, SIGNAL( relationChanged( QmvClass *) ),
             this, SIGNAL( relationChanged() ) );
    connect( query_object, SIGNAL( queryChanged() ),
             this, SLOT( updateRelationSet() ) );
    initSimple();
}




//--------------------------------------------------------------------------------
//! Constructor - from \ref QmvClass
/*!
  Construct using a \ref QmvClass object - will create the required \ref QmvSqlQuery object.

  \param cls    
  \param parent 
  \param name   
*/
//--------------------------------------------------------------------------------
QmvQueryWidget::QmvQueryWidget( QmvClass * cls, QWidget * parent, const char * name)
        : QFrame( parent, name), new_query_object(true)
{
    query_object = new QmvSqlQuery( cls, name );
    connect( query_object, SIGNAL( relationChanged( QmvClass *) ),
             this, SIGNAL( relationChanged() ) );
    connect( query_object, SIGNAL( queryChanged() ),
             this, SLOT( updateRelationSet() ) );
    initSimple();
}


//--------------------------------------------------------------------------------
//! Constructor - from \ref QmvQueryWidget
/*!
  Construct using an existing \ref QmvQueryWidget object

  \param qw     
  \param parent 
  \param name   
*/
//--------------------------------------------------------------------------------
QmvQueryWidget::QmvQueryWidget( QmvQueryWidget * qw, QWidget * parent, const char * name )
        : QFrame( parent, name), new_query_object(false)
{
    query_object = qw->getQueryObject();
    connect( query_object, SIGNAL( relationChanged( QmvClass *) ),
             this, SIGNAL( relationChanged() ) );
    connect( query_object, SIGNAL( queryChanged() ),
             this, SLOT( updateRelationSet() ) );
    initAdvanced();
}


//--------------------------------------------------------------------------------
// Destructor
//--------------------------------------------------------------------------------
QmvQueryWidget::~QmvQueryWidget()
{
    if ( new_query_object )
        delete query_object;
}


//--------------------------------------------------------------------------------
//! Get the query object
/*!

  \return       the underlying \ref QmvSqlQuery object
  
*/
//--------------------------------------------------------------------------------
QmvSqlQuery *  QmvQueryWidget::getQueryObject() const
{
    return query_object;
}

//--------------------------------------------------------------------------------
//! Clear the query object
/*!

*/
//--------------------------------------------------------------------------------
void  QmvQueryWidget::clearQueryObject()
{
    query_object->clear();
}


//--------------------------------------------------------------------------------
//! Create a WHERE button
/*!

  \param label          Button Label
  \param parent         parent widget
  \param name           object name

  \return               the create WHERE button
  
*/
//--------------------------------------------------------------------------------
QPushButton * QmvQueryWidget::createWhereButton( const QString & label, QWidget * parent, const char * name )
{
    QPushButton * pb_select = new QPushButton( label, parent, name );
    pb_select->setIconSet( QIconSet(QmvQueryWidgetIcons::find_xpm ));
    connect( pb_select, SIGNAL( clicked() ), this, SLOT( updateRelationSet() ) );
    pb_select->setAccel( SHIFT+Key_F5 ); 
    QWhatsThis::add( pb_select,
                     "<h3>Select where:</h3>"
                     "Activate a search using the current query conditions."
                     " (Shift F5)");
    return pb_select;
}
//--------------------------------------------------------------------------------
//! Create and Order-By widget
/*!

  \param parent         the parent widget
  \param name           object name
*/
//--------------------------------------------------------------------------------
void QmvQueryWidget::createOrderByComboBox( QWidget * parent, const char * name )
{
    cb_order_list = new QComboBox( TRUE, parent, name );
    cb_order_list->setEditable( TRUE );
    cb_order_list->setInsertionPolicy( QComboBox::NoInsertion );
    cb_order_list->setAutoCompletion( TRUE );
    cb_order_list->insertStringList( query_object->getAttributeTitles(), 0 );
    cb_order_list->insertItem( QString("Default"), 0);
    int label_pos = 0;
    label_pos = query_object->getCurrentOrderbyPosition();
    
    if ( label_pos < 0 )
        cb_order_list->setCurrentItem(0);
    else
        cb_order_list->setCurrentItem( label_pos );
    
    QWhatsThis::add( cb_order_list,
                     "<h3>Order by:</h3> Select an attribute to order the list.");
    
    connect( cb_order_list, SIGNAL( activated( int ) ),
             this, SLOT( orderbyChanged( int ) ) );
    connect( cb_order_list, SIGNAL( activated( const QString & ) ),
             this, SLOT( orderbyChanged( const QString & ) ) );
}

//--------------------------------------------------------------------------------
//! Create a simple query widget.
/*!
  Create a simple query widget based on a single attribute. Default query operator
  to "Like".
*/
//--------------------------------------------------------------------------------
void QmvQueryWidget::initSimple()
{
    query_icons = new QmvIcons( this, "querywidget-icons" );
    g_layout = new QGridLayout( this, 1, 3, 1, -1, "querywidget-v-layout" );
    QToolButton * tb_go;

        // Create the first query condition (and widget)
        // and hide it (it will be replaced here with single entry combo)
    
    int cond_count = initConditions( this, 1);
    getCondition(0)->setMaximumHeight(22);
    
        //------------------------------------------------------------
        // Attribute title
        //------------------------------------------------------------
    
    QmvClass * query_relation = query_object->getRelation();
    QString att_title("Browse where ");
    const QmvAttribute * att = query_relation->attribute( query_object->getTitleAt(0) );
    if ( att )
    {
            // Set the query attribute to userlabel attribute (via the condition widget,
            // to keep the advanced query in sync.
        getCondition(0)->atChanged( att->attributeTitle() );
    }
    
    tb_go = new QToolButton( this, "pattern label" );
    tb_go->setAutoRaise(FALSE);
    tb_go->setTextLabel("Load the search results");
    tb_go->setText( QString("%1 ").arg(att_title) );
    g_layout->addWidget( tb_go, 0, 0, 0 );
    tb_go->setAccel( SHIFT+Key_F5 );
    QWhatsThis::add(tb_go, QString( "<h3>Reload the search results:</h3>"
                                         "If the search field is empty, then search for any value."
                                         " (Shift F5)") );
    connect( tb_go, SIGNAL( clicked() ), this, SLOT( updateRelationSet() ) );
    
        //------------------------------------------------------------
        // Create a single query widget
        //------------------------------------------------------------
    QmvQueryConditionWidget * qcw_0 = getCondition(0);
    g_layout->addWidget( qcw_0, 0, 1, 0 );
    setFocusProxy( qcw_0 );

        //------------------------------------------------------------
        // Add button for advanced query parameters
        //------------------------------------------------------------

    QToolButton * pb_advqry = new QToolButton( this, "Advanced query parameters" );
    pb_advqry->setAutoRaise(TRUE);
    pb_advqry->setIconSet( QIconSet(QmvQueryWidgetIcons::find_xpm ));
    pb_advqry->setTextLabel(" Advanced Query (CTRL+SHIFT+ALT+Key_Q)");
    g_layout->addWidget( pb_advqry, 0, 2, 0 );
    pb_advqry->setAccel( CTRL+SHIFT+ALT+Key_Q ); 
    QWhatsThis::add(pb_advqry , QString( "<h3>Advanced query:</h3>"
                                         "Multiple query parameters."
                                         " (Ctrl+Shift+Alt+Q)") );
    
    connect( pb_advqry, SIGNAL( clicked() ), this, SLOT( advancedQuery()) );
   
        //------------------------------------------------------------
        // Create progress object for queries
        //------------------------------------------------------------

    controller = new QProgressDialog( QString("<h2>%1:</h2> Loading Records ...")
                                      .arg(query_object->getRelation()->relationTitle()),
                                      "Cancel", 100,
                                      this, "progress", TRUE );
    query_object->setProgressDialog( controller );
    connect( query_object, SIGNAL( queryStatus( int, QmvClass * ) ),
             this, SLOT( noticeQueryStatus( int, QmvClass * ) ) );
}



//--------------------------------------------------------------------------------
//! Initialise an advanced query widget
/*!
  
*/
//--------------------------------------------------------------------------------
void QmvQueryWidget::initAdvanced()
{
    query_icons = new QmvIcons( this, "querywidget-icons" );  
    QmvClass * query_relation = query_object->getRelation();
    dialog_advqry = new QDialog( this, "advanced query", TRUE, 0 );
    dialog_advqry->setCaption( "Advanced Query - " + query_object->getRelation()->relationTitle() );
    QVBoxLayout * vl_advqry = new QVBoxLayout( dialog_advqry, 2 );

    QHBoxLayout * hl_advqry = new QHBoxLayout( vl_advqry, 0, "controls_advqry" );
    QLabel * testlabel = new QLabel( "Advanced Query - " + query_relation->relationTitle(),
                                     dialog_advqry );
    hl_advqry->addWidget(testlabel);

        //------------------------------------------------------------
        // Control: close button
        //------------------------------------------------------------
    QPushButton * close_advqry = new QPushButton( "Close the advanced query window",
                                                  dialog_advqry,
                                                  "close_advqry");
    close_advqry->setPixmap( query_icons->getPixmap( QmvIcons::QuitIcon ) );
    close_advqry->setAccel( CTRL+Key_W );
    QWhatsThis::add( close_advqry, "<b>Close the advanced query window:</b>.");
    hl_advqry->addStretch( 10 );
    hl_advqry->addWidget(close_advqry);
    close_advqry->setDefault(FALSE);
    close_advqry->setFocusPolicy( QWidget::NoFocus );
    connect( close_advqry, SIGNAL( clicked() ), dialog_advqry, SLOT( accept() ) );

        // dummy button to catch QDialog return key
        // (return is used to activate the search)
    QPushButton * dummy = new QPushButton( "", dialog_advqry, "");
    dummy->hide();
    dummy->setDefault(TRUE);
    
        // add stretch between top and header
    vl_advqry->addStretch();

    
        //---------------------------
        // Create Select (GO) button
        //---------------------------
    QHBoxLayout * cond_layout = new QHBoxLayout( vl_advqry, 0, "condition" );
    QPushButton * pb_select = createWhereButton( "Find matching rows:", dialog_advqry, "fieldlabel" );
    cond_layout->addWidget( pb_select );
    pb_select->setDefault(FALSE);
    pb_select->setFocusPolicy( QWidget::NoFocus );;
    
    
        //------------------------------------------------------------
        // Add condition widgets
        //------------------------------------------------------------
    int cond_size = 4;
    int base_cond_count = query_object->countConditions();
    initConditions( dialog_advqry, cond_size);
    for ( int cond = 0; cond < cond_size; cond++)
    {
        QmvQueryConditionWidget * cw = getCondition(cond);   
        cond_layout = new QHBoxLayout( vl_advqry, 0, "condition" ); 
        cond_layout->addWidget( cw );
            // only set if a new widget - the base widget may have been altered.
            // :: query_object->getTitleAt(cond) returns from the original unaltered attribute list.
        if ( cond >= base_cond_count )
            cw->atChanged( query_object->getTitleAt(cond) );
    }
    
    

        //------------------------------------------------------------
        // Add horizontal layout for header widgets - order,limit,offset
        //------------------------------------------------------------
    QHBoxLayout * hl_header_advqry = new QHBoxLayout( vl_advqry, 0, "limit and offset" );
    

        //-------------------------------
        // Create combobox for order by
        //-------------------------------
    QLabel * lab = new QLabel( "Order by:", dialog_advqry, "orderlabel" );
    hl_header_advqry->addWidget( lab );
    createOrderByComboBox( dialog_advqry, "value_combo");
    hl_header_advqry->addWidget( cb_order_list);
    
        
        //-------------------------------
        // Add limit selector
        //-------------------------------
    lab = new QLabel( "Row Limit:", dialog_advqry, "limitlabel" );
    hl_header_advqry->addWidget( lab );
    sb_query_limit = new QSpinBox ( 0, 1000, 10, dialog_advqry, "querylimit" );
    hl_header_advqry->addWidget( sb_query_limit );
    sb_query_limit->setSpecialValueText( "none" );
    sb_query_limit->setValue( query_object->getLimit() );
    connect( sb_query_limit, SIGNAL( valueChanged( int ) ),
             query_object, SLOT( setLimit( int ) ) );

        //----------------------
        // Add offset selector
        //----------------------
    lab = new QLabel( "Offset:", dialog_advqry, "offsetlabel" );
    hl_header_advqry->addWidget( lab );
    sb_query_offset = new QSpinBox ( 0, 1000, 10, dialog_advqry, "queryoffset" );
    hl_header_advqry->addWidget( sb_query_offset );
    sb_query_offset->setValue( query_object->getOffset() );
    connect( sb_query_offset, SIGNAL( valueChanged( int ) ),
             query_object, SLOT( setOffset( int ) ) );


    getCondition(0)->setFocus();
}



//--------------------------------------------------------------------------------
//! Set the base query
/*!

  \param query          the base query (to be and'ed with all queries)
*/
//--------------------------------------------------------------------------------
void QmvQueryWidget::setBaseQuery( const QString & query )
{
    query_object->setBaseQuery( query );
}

//--------------------------------------------------------------------------------
//! Get the base query
/*!

*/
//--------------------------------------------------------------------------------
const QString QmvQueryWidget::getBaseQuery() const
{
    return query_object->getBaseQuery();
}



//--------------------------------------------------------------------------------
//! Initialise condition widgets
/*!

  Create a set of condition widgets for the current set of query
  conditions.  If the current number of query conditions is less than
  \ref minsize, the number of conditions will be extended.

  \param parent         the parent query widget
  \param minsize        minimum required number of conditions

  \return               the current number of conditions.
  
*/
//--------------------------------------------------------------------------------
int QmvQueryWidget::initConditions( QWidget * parent, int minsize )
{
        // clear existing condition widgets (but not the condition objects)
    condition_list.setAutoDelete(TRUE);
    condition_list.clear();

        // Scan existing condition objects and create widgets for them
    for ( int row = 0; row < query_object->countConditions(); row++ )
    {
        QmvQueryCondition * query_condition = query_object->getCondition( row );
        query_condition->setActive(TRUE);
        QmvQueryConditionWidget * cw;
        cw = new QmvQueryConditionWidget( query_condition,
                                          parent,
                                          QString( "conditionwidget-%1" )
                                          .arg(row) );
        condition_list.insert( query_condition, cw );
    }
        // Extend the condition list to minsize with new condition
        // objects, complete with matching condition widgets
    for ( int row = condition_list.count(); row < minsize; row++)
        addCondition( parent, row);

    return condition_list.count();
}

//--------------------------------------------------------------------------------
//! Get the pos'th \ref QmvQueryConditionWidget.
/*!

  \param pos    the index of required QmvQueryConditionWidget
  \return       the QmvQueryConditionWidget at index \ref pos
  
*/
//--------------------------------------------------------------------------------
QmvQueryConditionWidget * QmvQueryWidget::getCondition( int pos ) const
{
    QmvQueryCondition * query_condition = query_object->getCondition( pos );
    return condition_list.find( query_condition );
}


//--------------------------------------------------------------------------------
//! Add a new condition - with a widget.
/*!

  \param parent         the parent query widget
  \param row            the index for the new widget.
  \return               pointer to the new QmvQueryConditionWidget.
*/
//--------------------------------------------------------------------------------
QmvQueryConditionWidget * QmvQueryWidget::addCondition( QWidget * parent, int row )
    
{
    QmvQueryCondition * query_condition = query_object->addCondition();
    QmvQueryConditionWidget * cw = new QmvQueryConditionWidget( query_condition,
                                                      parent, QString( "conditionwidget-%1" ).arg(row) );
    condition_list.insert( query_condition, cw );
    query_condition->setActive(TRUE);
    return cw;
}

//--------------------------------------------------------------------------------

//! Respond to changed current query status
/*!

  This slot responds to the current query status. The slot should be
  called after a query has completeed or failed. It will popup a
  dialogue in the case of a error, and reset and hide the progress
  dialog (\ref controller)

  \param status         -1 = fail, -2 = cancelled, otherwise success
  \param cls         the relation object
*/
//--------------------------------------------------------------------------------
void QmvQueryWidget::noticeQueryStatus( int status, QmvClass * cls )
{

    switch ( status )
    {
        case -1:
            QMessageBox::warning( 0, "warning",
                                  tr("<b>%1:</b><br>"
                                     "The data query failed. The following message"
                                     " may help to explain the failure.<br><br>"
                                     "SQL: %2")
                                  .arg( cls->relationTitle() )
                                  .arg( cls->lastError() ),
                                  "OK", 0 );
            break;
        case -2:
            QMessageBox::warning( 0, "warning",
                                  tr("<b>%1:</b><br>"
                                     "The data query has been cancelled as requested.")
                                  .arg( cls->relationTitle() ),
                                  "OK", 0 );
            break;
        default:
            break;
    }
}



//--------------------------------------------------------------------------------
//! Slot to handle changes in the simple field widget
/*!
    
    
*/
//--------------------------------------------------------------------------------
void QmvQueryWidget::simpleQueryChanged( const QString & text )
{
    
        // update the query_object
    QmvQueryCondition * qc = getQueryObject()->getCondition(0);
    if ( qc )
        qc->setPatternValue( text, TRUE );
    
}



//--------------------------------------------------------------------------------
//! Slot to handle changes in order-by (via index)
/*!

  Change the current Order-by and update the current relation set.

  \param pos    index in the order-by set.
  
*/
//--------------------------------------------------------------------------------
void QmvQueryWidget::orderbyChanged( int pos )
{
    QmvClass * query_relation = query_object->getRelation();
    QString orderby_str = query_relation->attributeNamebyTitle( cb_order_list->text( pos )  );
    query_object->setOrderby( orderby_str );
    cb_order_list->setEditText( cb_order_list->text( cb_order_list->currentItem() ) );
    updateRelationSet();
}

//--------------------------------------------------------------------------------
//! Slot to handle changes in order-by (via text)
/*!

  Make sure the order-by is a valid member of cb_order_list, and then set that to
  be the current item. Otherwise leave the current item order-by unchanged.
  
  \param order_text     the text of the new order-by
  
*/
//--------------------------------------------------------------------------------
void QmvQueryWidget::orderbyChanged( const QString & order_text )
{
    for ( int pos = 0; pos < cb_order_list->count(); pos++ )
    {
        if ( cb_order_list->text(pos) != order_text )
            continue;
        orderbyChanged( pos );
        cb_order_list->setCurrentItem( pos );
        return;
    }
    cb_order_list->setEditText( cb_order_list->text( cb_order_list->currentItem() ) );
}

//--------------------------------------------------------------------------------
//! Slot to update relation set
/*!

  Update the relation set to match current query state

*/
//--------------------------------------------------------------------------------
void QmvQueryWidget::updateRelationSet()
{

    bool ok = false;
    int query_limit = query_object->getRelation()->queryLimit().toInt(&ok);
    if (!ok) {
        query_limit = 0;
    }
    
    int estimate = query_object->countEstimate();
    if (query_limit > 0 && estimate > query_limit ) {
        QString msg = QString("<h2>Are you sure that you want to proceed with this query ?</h2> \n"
                              "This query may result in a lengthy search (<b>an estimated %1 rows</b>)\n\n"
                              "If you decide to proceed, you can cancel the query at any time.")
            .arg(estimate);

        int status =
            QMessageBox::information( this,
                                      QString("Warning of possibly long search: %1")
                                      .arg(query_object->getRelation()->relationTitle()),
                                      msg,
                                      "&Yes", "&Cancel", QString::null,
                                      0, 1 );
        switch(status)                                   
        {
            case 0:
                break;
            case 1:
                return;
                break;
            default: // just for sanity
                break;
        }
    }

    query_object->updateRelationSet();
}

//--------------------------------------------------------------------------------
//! Change the active state of query conditions
/*!
  Change the state of query conditions to \em state.

  \param state  
*/
//--------------------------------------------------------------------------------
void QmvQueryWidget::setActive( bool state )
{
    for ( int row = 0; row < query_object->countConditions(); row++ )
        query_object->getCondition( row )->setActive(state);
}

    
//--------------------------------------------------------------------------------
//! Display the advanced query dialog
/*!
  Create the dialog if it does not already exist.

*/
//--------------------------------------------------------------------------------
void QmvQueryWidget::advancedQuery()
{
    static QmvQueryWidget * aq;
    if ( !dialog_advqry )
    {
        aq = new QmvQueryWidget( this, this, "advanced query" );
        dialog_advqry = aq->getAdvancedDialog();
        dialog_advqry->move( mapToGlobal( QPoint( topLevelWidget()->width() - 400, 0) ) );
    }
        // update adv from simple
    aq->getCondition(0)->update();
        // activate all conditions
    for ( int row = 0; row < query_object->countConditions(); row++ )
        query_object->getCondition( row )->setActive(TRUE);

        // Run the advanced query
    dialog_advqry->exec();
    
        // update simple from adv
    getCondition(0)->update();
        // deactivate all other than first condition
    for ( int row = 1; row < query_object->countConditions(); row++ )
        query_object->getCondition( row )->setActive(FALSE);
    
}


//--------------------------------------------------------------------------------
//! Return pointer to advanced query dialog.
/*!
  Return a pointer to the advanced query dialog.

*/
//--------------------------------------------------------------------------------
QDialog * QmvQueryWidget::getAdvancedDialog() const
{
    return dialog_advqry;
}
