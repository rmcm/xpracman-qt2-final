--  -*- sql -*-
--
 
--    $Id: mtcl-crea.sql 2030 2004-01-19 00:30:25Z source $

--  Copyright 2001 X=X Computer Software Trust
--   		  Kangaroo Ground Australia 3097


-- This is free software; you can redistribute it and/or modify
-- it under the terms of the GNU General Public Licensepublished by
-- the Free Software Foundation; either version 2, or (at your option)
-- any later version.
 
-- This software is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- GNU General Public License for more details.
 
-- You should have received a copy of the GNU General Public License
-- along with this software; see the file COPYING.  If not, write to
-- the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 
-- Report problems and direct all questions to:
 
--     Rex McMaster, rmcm@compsoft.com.au
 

 
-- Revision Log
  

drop table mtcl;
drop sequence mtcl_mtcl__sequence_seq;
create table mtcl                       -- CLASS meta data
(

 mtcl_access            text default text '2' not null,     -- access list to match phplib access
 mtcl_name              text default text 'NAME' not null,  -- name of class
 mtcl_title             text default text 'TITLE' not null, -- descriptive title for class
 mtcl_group             text references mtag ON DELETE set null ON UPDATE cascade,
                                                            -- group name for clustering classes on matrix
 mtcl_matrix_order      integer,                            -- order within group
 mtcl_order_by          text,                               -- attribute list to use in default order-by
                                                            --  clause in selects
 mtcl_group_by          text,                               -- default display grouping attribute
                                                            -- (eg in browse lists)
 mtcl_primary           text,                               -- primary key
 mtcl_userkey           text,                               -- user unique key attribute
 mtcl_userlabel         text,                               -- user label attribute
 mtcl_query_limit       integer default 200,                -- default query limit
 mtcl_query_offset      integer default 0,                  -- default query offset
 mtcl_extras            text,                               -- extra class files (eg MTCL_extras.inc) 
                                                            -- comma-sep list

 --
 -- standard headers
 --
 mtcl__sequence serial not null,                            -- Row-ID
 mtcl__timestamp timestamp default text 'now' not null,     -- Timestamp
 mtcl__user_entry text default CURRENT_USER not null,       -- Entered by
 mtcl__status char default text 'A' not null,               -- Record status
 --
 -- constraints
 --
 primary key (mtcl_name)
 );


grant select,insert,update on mtcl to public;
-- this has to be granted for TABLE_insert_row() to function
grant select,insert,update on mtcl_mtcl__sequence_seq to public;
