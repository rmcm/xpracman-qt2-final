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
create table quit                       -- Quotation Items
(
 quit_date_service timestamp default now() not null,                    -- Date of service
 quit_serv_code text,                                                   -- Service code
 quit_multireduction text default '-' not null,                         -- Multireduction flag
 quit_percentage integer default 100 not null,                          -- Fee percentage
 quit_desc text,                                                        -- Description
 quit_amount numeric(12,2) default numeric(12,2) '0.0' not null,        -- Amount for patients feetype
 quit_amount_mbs100 numeric(12,2) default numeric(12,2) '0.0' not null, -- MBS Amount -100%
 quit_amount_mbs85 numeric(12,2) default numeric(12,2) '0.0' not null,  -- REBATE Amount
 quit_qute__sequence integer,                                           -- Quotation number
 quit_patn__sequence integer,                                           -- Patient
 --
 -- standard headers
 --
 quit__sequence serial not null,                                        -- Row-ID
 quit__timestamp timestamp default now() not null,                      -- Timestamp
 quit__user_entry text default CURRENT_USER not null,                   -- Entered by
 quit__status char default text 'N' not null,                           -- Record status
 --
 -- constraints
 --
 primary key(quit__sequence),
 constraint quit_fk_qute
            foreign key(quit_qute__sequence)
            references qute
            on update cascade
            on delete cascade
            initially deferred,
 constraint quit_fk_patn
            foreign key(quit_patn__sequence)
            references patn
            on update cascade
            on delete cascade
            initially deferred,
 constraint quit_fk_serv
            foreign key(quit_serv_code)
            references serv
            on update cascade
            on delete restrict
            initially deferred

 );

-- ------------------------------------------------------------
-- FUNCTIONS
-- ------------------------------------------------------------

-- ------------------------------------------------------------
-- Insert a row and return the PK sequence id (used by Marvin)
-- ------------------------------------------------------------
create or replace function quit_insert_row()
returns integer as $$
    DECLARE
      rec_quit quit%ROWTYPE;

    BEGIN
      
      -- add the quit record 
      select    *
      into      rec_quit
      from      quit_insert_basic_row();

      return rec_quit.quit__sequence;
    END;
$$
LANGUAGE 'plpgsql';

-- ------------------------------------------------------------
-- Insert a row and return the underlying row
-- ------------------------------------------------------------
create or replace function quit_insert_basic_row()
returns quit as $$
    DECLARE
      rec_quit quit%ROWTYPE;
      x_oid oid;

    BEGIN
      
         -- add the quit record 
         insert into quit default values;

          GET DIAGNOSTICS x_oid = RESULT_OID;
          select  *
          into    rec_quit
          from    quit
          where   oid = x_oid;
          if (not FOUND) then
            RAISE EXCEPTION 'QUIT:: Insert failed';
            return null;
          end if;

      return rec_quit;
    END;
$$
LANGUAGE 'plpgsql';


-- ------------------------------------------------------------
-- Permissions
-- ------------------------------------------------------------
grant select,insert,update on quit to public;
grant select,insert,update on quit_quit__sequence_seq to public;

-- ------------------------------------------------------------
-- Metadata
-- ------------------------------------------------------------
select create_metadata('quit');
update mtcl set mtcl_title = 'Quotation Items - base' where mtcl_name = 'quit';

-- update history;
\set mttb_name '\'quit\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;

