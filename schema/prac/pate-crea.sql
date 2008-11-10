--     $Id$
-- 
--   Copyright 2004 X=X Computer Software Trust
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



create table pate                       -- Patient Export Table -eg for MD2
(
 pate_patn__sequence integer,                           -- Patient
 --
 -- standard headers
 --
 pate__sequence serial not null,                        -- Row-ID
 pate__timestamp timestamp default now() not null,      -- Timestamp
 pate__user_entry text default CURRENT_USER not null,   -- Entered by
 pate__status char default text 'N' not null,           -- Record status
 --
 -- constraints
 --
 primary key(pate__sequence)
 );

create unique index ix_patn__sequence on pate(pate_patn__sequence);
grant select,insert,update on pate to public;
grant select,insert,update on pate_pate__sequence_seq to public;

select create_metadata('pate');
update mtcl set mtcl_title = 'Patients Edited' where mtcl_name = 'pate';

-- create a trigger to fill this table with patn updates

create or replace function fn_patientexport()
returns trigger
as 'DECLARE

    BEGIN

        if (new.patn__sequence > 0 ) then
          insert into pate(pate_patn__sequence)
          select new.patn__sequence
          except  select  pate_patn__sequence
                  from    pate;
        end if;
    return new;
    END;'
  LANGUAGE 'plpgsql';

drop trigger tr_patientexport on patn;
create trigger tr_patientexport after insert or update
    on patn for each row
    execute procedure fn_patientexport();

-- update history;
\set mttb_name '\'pate\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;

