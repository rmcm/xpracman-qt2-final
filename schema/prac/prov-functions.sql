--  -*- sql -*-
--
 
--    $Id$

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
  
-- ------------------------------------------------------------
-- Function to insert a new record with dummy prov_code
-- ------------------------------------------------------------

create or replace function prov_insert_row()
returns integer
as 'DECLARE
    -- NAME TABLE.ATTRIBUTE%TYPE;
    -- NAME TABLE%ROWTYPE;
    tmp_prov__sequence prov.prov__sequence%TYPE;

    BEGIN

         -- get a prov__sequence
         select nextval(''prov_prov__sequence_seq'')
         into tmp_prov__sequence;

         -- add the new row
         insert into prov( prov_code, prov__sequence )
                   values( ''NEW-''||tmp_prov__sequence::text, tmp_prov__sequence);

    return tmp_prov__sequence;
    END;'
  LANGUAGE 'plpgsql';


-- update history;
\set mttb_name '\'prov\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;
