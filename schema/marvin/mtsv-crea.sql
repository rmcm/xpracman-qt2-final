--  -*- sql -*-
--  
--    $Id: mtsv-crea.sql 2428 2005-02-05 23:19:24Z source $

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
 
-- Function to extract simple and compound keys from pg_trigger bytea

drop view mtsv;
drop function mtsv_keys(bytea,int,int);
create function mtsv_keys(bytea,int,int)
returns text
as 'DECLARE
    x_tgargs alias for $1;
    x_tgnargs alias for $2;
    x_offset alias for $3;
    x_keys text;
    x_sep text;
    x_count integer;

    BEGIN
    
    x_keys := '''';
    x_sep := '''';
    x_count = x_offset;

    while x_count < x_tgnargs
      LOOP
        x_keys := x_keys || x_sep || byteaparam(x_tgargs, x_count);
        x_count := x_count + 2;
        x_sep := '','';
      END LOOP;
        
    return x_keys;
    END;'
  LANGUAGE 'plpgsql';


create view mtsv                -- Metadata - view of relations via system tables
 as select 
      --field order is 0. procName
      --               1. referringTable
      --               2. localTable
      --               3. UNSPECIFIED
      --               4. referringField
      --               5. localField
 byteaparam(pg_trigger.tgargs, 0)                       as mtsv_name,
 byteaparam(pg_trigger.tgargs, 1)                       as mtsv_master_class ,
 mtsv_keys(pg_trigger.tgargs,pg_trigger.tgnargs,4)      as mtsv_key,
 byteaparam(pg_trigger.tgargs, 2)                       as mtsv_other_class,
 mtsv_keys(pg_trigger.tgargs,pg_trigger.tgnargs,5)      as mtsv_join,
 (case when pg_trigger.tgtype = 9
        then 'D'
  when pg_trigger.tgtype = 17
        then 'U'
  when pg_trigger.tgtype = 21
        then 'O'
  else '?'
  end)                                                  as mtsv_type,
 --
 -- standard headers
 --
 pg_trigger.oid                         as mtsv__sequence,              -- Row-ID
 now()                                  as mtsv__timestamp,             -- Timestamp
 CURRENT_USER                           as mtsv__user_entry,            -- Entered by
 'V'::char(1)                           as mtsv__status                 -- Record status
 --
 from     pg_trigger
 where    pg_trigger.tgnargs >= 6
 and      pg_trigger.tgisconstraint = true
;

grant select on mtsv to public;

select create_metadata('mtsv');
update mtcl set mtcl_title = 'PRAC Trigger View', mtcl_order_by = 'order by mtsv_master_class, mtsv_key' where mtcl_name = 'mtsv';

-- update history;
\set mttb_name '\'mtsv\''
\set mttb_rcs_header '\'$Id: mtsv-crea.sql 2428 2005-02-05 23:19:24Z source $\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;

