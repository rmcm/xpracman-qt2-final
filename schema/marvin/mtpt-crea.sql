--  -*- sql -*-
--
 
--    $Id: mtpt-crea.sql 1406 2002-09-26 08:25:41Z source $

--  Copyright 2002 X=X Computer Software Trust
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
  

drop table mtpt;
drop sequence mtpt_mtpt__sequence_seq;
create table mtpt                       -- CLASS meta - patch history
(

 mtpt_name              text default text 'NAME' not null,  -- name of patch
 mtpt_rcs_header        text default text 'RCS HEADER' not null, -- RCS header tag

 --
 -- standard headers
 --
 mtpt__sequence serial not null,                            -- Row-ID
 mtpt__timestamp timestamp default text 'now' not null,     -- Timestamp
 mtpt__user_entry text default CURRENT_USER not null,       -- Entered by
 mtpt__status char default text 'A' not null,               -- Record status
 --
 -- constraints
 --
 primary key (mtpt__sequence)
 );


grant select,insert,update on mtpt to public;
-- this has to be granted for TABLE_insert_row() to function
grant select,insert,update on mtpt_mtpt__sequence_seq to public;
