--  -*- sql -*-
--
--     $Id$

--   Copyright 2001 X=X Computer Software Trust
--    		  Kangaroo Ground Australia 3097


--  This is free software; you can redistribute it and/or modify
--  it under the terms of the GNU General Public Licensepublished by
--  the Free Software Foundation; either version 2, or (at your option)
--  any later version.
 
--  This software is distributed in the hope that it will be useful,
--  but WITHOUT ANY WARRANTY; without even the implied warranty of
--  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
--  GNU General Public License for more details.
 
--  You should have received a copy of the GNU General Public License
--  along with this software; see the file COPYING.  If not, write to
--  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 
--  Report problems and direct all questions to:
 
--      Rex McMaster, rmcm@compsoft.com.au
 

 
--  Revision Log
  


create table docs                                               -- Patient documents
(
 docs_patn__sequence integer,                                   -- Patient id
 docs_url text,                                                 -- URL of document
 docs_title text  default text 'UNNAMED' not null,              -- document title
 docs_mime_type text,                                           -- MIME type
 --
 -- standard headers
 --
 docs__sequence serial not null,                                -- Row-ID
 docs__timestamp timestamp default now() not null,              -- Timestamp
 docs__user_entry text default CURRENT_USER not null,           -- Entered by
 docs__status char default text 'N' not null,                   -- Record status
 --
 -- constraints
 --
 primary key (docs__sequence),
 constraint docs_fk_patn
            foreign key (docs_patn__sequence)
            references patn
            on update cascade
            on delete cascade
            initially deferred
);


grant select,insert,update, delete on docs to public;

select create_metadata('docs');
update mtcl set mtcl_title = 'Documnets', mtcl_order_by = 'order by docs_patn__sequence,docs__timestamp', mtcl_userlabel = '<b>Patient ID: %1</b> %2, docs_patn__sequence,docs__timestamp' where mtcl_name = 'docs';
-- update mtat set mtat_title = '_@ATT_TITLE@_', mtat_displen = _@ATT_DISPLEN@_, mtat_view_order = _@ATT_VIEW_ORDER@_, mtat_input_method = '_@ATT_INPUT_METHOD@_'  where mtat_name = '_@ATT_NAME@_';

update mtat set mtat_title = 'Patient ID', mtat_displen = 10, mtat_view_order = 10, mtat_input_method = '10'  where mtat_name = 'docs_patn__sequence';
update mtat set mtat_title = 'Title', mtat_displen = 30, mtat_view_order = 20, mtat_input_method = '20'  where mtat_name = 'docs_title';
update mtat set mtat_title = 'Location', mtat_displen = 30, mtat_view_order = 30, mtat_input_method = '30'  where mtat_name = 'docs_url';
update mtat set mtat_title = 'MIME-Type', mtat_displen = 20, mtat_view_order = 25, mtat_input_method = '40'  where mtat_name = 'docs_mime_type';
update mtat set mtat_title = 'Document ID', mtat_displen = 10, mtat_view_order = 40, mtat_input_method = 'RO'  where mtat_name = 'docs__sequence';
update mtat set mtat_title = 'Creation time', mtat_displen = 47, mtat_view_order = 50, mtat_input_method = 'RO'  where mtat_name = 'docs__timestamp';
update mtat set mtat_title = 'Created by', mtat_displen = 30, mtat_view_order = 60, mtat_input_method = 'RO'  where mtat_name = 'docs__user_entry';
update mtat set mtat_title = 'Status', mtat_displen = 1, mtat_view_order = 70, mtat_input_method = 'SYSTEM'  where mtat_name = 'docs__status';


-- update history;

\set mttb_name '\'docs\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;
