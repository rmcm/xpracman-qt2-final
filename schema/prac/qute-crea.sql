--     $Id$
-- 
--   Copyright 2005 X=X Computer Software Trust
--    		  Kangaroo Ground Australia 3097
-- 
-- 
--  This is free software; you can redistribute it and/or modify
--  it under the terms of the GNU General Public License published by
--  the Free Software Foundation; either version 2, or (at your option)
--  any later version.
--  
--  This software is distributed in the hope that it will be useful,
--  but WITHOUT ANY WARRANTY; without even the implied warranty of
--  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
--  GNU General Public License for more details.
--  
--  You should have received a copy of the GNU General Public License
--  along with this software; see the file COPYING.  If not, write to
--  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
--  
--  Report problems and direct all questions to:
--  
--      Rex McMaster, rmcm@compsoft.com.au
--  
-- 

-- ------------------------------------------------------------
-- Create the table
-- ------------------------------------------------------------
create table qute                                       -- Patient Quotes - base
(
 qute_desc text,                                        -- description
 qute_date_created timestamp default now(),             -- created
 qute_patn__sequence integer,                           -- patient quoted
 qute_feet_code text default '-',                       -- fee level of quote
 --
 -- standard headers
 --
 qute__sequence serial not null,                        -- Row-ID
 qute__timestamp timestamp default now() not null,      -- Timestamp
 qute__user_entry text default CURRENT_USER not null,   -- Entered by
 qute__status char default text 'N' not null,           -- Record status
 --
 -- constraints
 --
 primary key(qute__sequence),
 constraint qute_fk_patn
            foreign key(qute_patn__sequence)
            references patn
            on update cascade
            on delete cascade
            initially deferred,
 constraint qute_fk_feet
            foreign key(qute_feet_code)
            references feet
            on update cascade
            on delete cascade
            initially deferred

 );

-- ------------------------------------------------------------
-- FUNCTIONS
-- ------------------------------------------------------------

-- ------------------------------------------------------------
-- Insert a row and return the PK sequence id (used by Marvin)
-- ------------------------------------------------------------
create or replace function qute_insert_row()
returns integer as $$
    DECLARE
      rec_qute qute%ROWTYPE;

    BEGIN
      
      -- add the qute record 
      select    *
      into      rec_qute
      from      qute_insert_basic_row();

      return rec_qute.qute__sequence;
    END;
$$
LANGUAGE 'plpgsql';

-- ------------------------------------------------------------
-- Insert a row and return the underlying row
-- ------------------------------------------------------------
create or replace function qute_insert_basic_row()
returns qute as $$
    DECLARE
      rec_qute qute%ROWTYPE;
      x_oid oid;

    BEGIN
      
         -- add the qute record 
         insert into qute default values;

          GET DIAGNOSTICS x_oid = RESULT_OID;
          select  *
          into    rec_qute
          from    qute
          where   oid = x_oid;
          if (not FOUND) then
            RAISE EXCEPTION 'QUTE:: Insert failed';
            return null;
          end if;

      return rec_qute;
    END;
$$
LANGUAGE 'plpgsql';


-- ------------------------------------------------------------
-- PERMISSIONS
-- ------------------------------------------------------------

grant select,insert,update on qute to public;
grant select,insert,update on qute_qute__sequence_seq to public;

-- ------------------------------------------------------------
-- METADATA
-- ------------------------------------------------------------
select create_metadata('qute');
update mtcl set mtcl_title = 'Patient Quotes - base' where mtcl_name = 'qute';

-- update history;
\set mttb_name '\'qute\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;


