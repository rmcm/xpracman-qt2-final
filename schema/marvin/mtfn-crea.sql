--  -*- sql -*-
--
 
--    $Id: mtfn-crea.sql 1288 2002-04-30 00:01:29Z source $

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
  

drop table mtfn;
drop sequence mtfn_mtfn__sequence_seq;
create table mtfn                       -- ATTRIBUTE meta data
(
 mtfn_title           text default text 'TITLE' not null,  -- text description of the foreign class
                                                           -- in this context.
 mtfn_master_class    text default text '-' not null,      -- the master class
 mtfn_key             text default text '-' not null,      -- the attribute in the master class
                                                           -- used as key
 mtfn_other_class     text default text '-' not null,      -- the foreign class
 mtfn_join            text default text '-' not null,      -- the key attribute in the other class
 mtfn_view            text default text '-' not null,      -- an array (comma-separated list) of one or
                                                           --  more display attributes in the foreign class

--
 -- standard headers
 --
 mtfn__sequence serial not null,                            -- Row-ID
 mtfn__timestamp timestamp default text 'now' not null,     -- Timestamp
 mtfn__user_entry text default CURRENT_USER not null,       -- Entered by
 mtfn__status char default text 'A' not null,               -- Record status
 --
 -- constraints
 --
 primary key (mtfn__sequence)
 );

grant select,insert,update on mtfn to public;
-- this has to be granted for TABLE_insert_row() to function
grant select,insert,update on mtfn_mtfn__sequence_seq to public;
