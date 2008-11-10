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



-- Create VIew "pypv" based on "prov"
--

-- \i schema/DBNAME/pypv-functions.sql   -- Functions need to be defined before the VIEW

drop view pypv;
create view pypv                         -- Payee Provider
  as select
 prov_code                              as pypv_prov_code,
 prov_provider_num                      as pypv_prov_provider_num,
 prov_name                              as pypv_prov_name,
 prov_address                           as pypv_prov_address,
 prov_suburb                            as pypv_prov_suburb,
 prov_state                             as pypv_prov_state,
 prov_postcode                          as pypv_prov_postcode,
 prov_phone                             as pypv_prov_phone,

--
-- Bank
--

 bank_name                              as pypv_bank_name,
 bank_bsb                               as pypv_bank_bsb,
 bank_account                           as pypv_bank_account,

--
-- standard headers
--
 prov__sequence                         as pypv__sequence,              -- Row-ID
 prov__timestamp                        as pypv__timestamp,             -- Timestamp
 prov__user_entry                       as pypv__user_entry,            -- Entered by
 prov__status                           as pypv__status                 -- Record status
 --
 from prov LEFT JOIN bank ON (bank.bank_code = prov.prov_bank_code)
;

grant select,insert,update on pypv to public;

-- Rules

-- Insert Rule
-- The insert is more flexibly handled by a function;
--    eg pypv_insert_row() in pypv-functions.sql

-- Update rule
-- drop rule rl_pypv_update;
-- create rule rl_pypv_update as on update
--        to pypv
--        do instead
--        (
--         update prov
--         set    prov_  = new.pypv_
--         where  prov_  = new.pypv_
--        );
-- 
-- -- Delete rule
-- drop rule rl_pypv_delete;
-- create rule rl_pypv_delete as on delete
--        to pypv
--        do instead
--        (
--         delete from prov
--         where  prov_  = old.pypv_
--        );
-- 
-- add the default metadata
select create_metadata('pypv');
update mtcl set mtcl_title = 'Payee Provider' where mtcl_name = 'pypv';

-- update history;
\set mttb_name '\'pypv\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;

