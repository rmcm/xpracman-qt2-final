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



-- Create VIew "feel" based on "feet"
--

-- \i schema/DBNAME/feel-functions.sql   -- Functions need to be defined before the VIEW

-- ------------------------------------------------------------
-- Get fees
-- ------------------------------------------------------------
create or replace function getfee(serv.serv_code%TYPE, integer)
RETURNS numeric AS $$

  select    feeb_amount
  from      feeb
  where     feeb_serv_code = $1
  and       feeb_feet_code = getfeetype($2);

$$
LANGUAGE SQL;

-- ------------------------------------------------------------
-- Get fee level code
-- ------------------------------------------------------------
create or replace function getfeetype(integer)
RETURNS text AS $$
  select    feet_code
  from      feet
  where     feet_code = (string_to_array((select conf_value from conf where conf_code = 'FEEL_CODE_LIST'),','))[$1+1];

$$
LANGUAGE SQL;

-- ------------------------------------------------------------
-- Create view for fee list
-- ------------------------------------------------------------
create or replace view feel                         -- Fee List View
  as SELECT
 
 (select db_desc from mvac_database)    as feel_db_desc,
 serv_code                              as feel_serv_code,
 serv_desc                              as feeL_serv_desc,
 getfeetype(0)                          as feel_fee0_code,
 getfee(serv_code, 0)                   as feel_fee0_amount,
 getfeetype(1)                          as feel_fee1_code,
 getfee(serv_code, 1)                   as feel_fee1_amount,
 getfeetype(2)                          as feel_fee2_code,
 getfee(serv_code, 2)                   as feel_fee2_amount,
 getfeetype(3)                          as feel_fee3_code,
 getfee(serv_code, 3)                   as feel_fee3_amount,
 getfeetype(4)                          as feel_fee4_code,
 getfee(serv_code, 4)                   as feel_fee4_amount,
 getfeetype(5)                          as feel_fee5_code,
 getfee(serv_code, 5)                   as feel_fee5_amount,
 getfeetype(6)                          as feel_fee6_code,
 getfee(serv_code, 6)                   as feel_fee6_amount,
 getfeetype(7)                          as feel_fee7_code,
 getfee(serv_code, 7)                   as feel_fee7_amount,
 getfeetype(8)                          as feel_fee8_code,
 getfee(serv_code, 8)                   as feel_fee8_amount,

-- ----------------------------------------
-- standard headers
-- ----------------------------------------
 serv__sequence                         as feel__sequence,              -- Row-ID
 serv__timestamp                        as feel__timestamp,             -- Timestamp
 serv__user_entry                       as feel__user_entry,            -- Entered by
 serv__status                           as feel__status                 -- Record status
 --
 from serv

;

grant select,insert,update on feel to public;


-- add the default metadata
select create_metadata('feel');

update mtcl set mtcl_title = 'Fee Report', mtcl_query_limit = 0 where mtcl_name = 'feel';
update mtat set mtat_displen = 12 where mtat_class_name = 'feel';

-- add conf entry

insert  into conf(conf_code,conf_desc,conf_value)
select  'FEEL_CODE_LIST', 'Display Columns for Fee Report', 'SCH,REB,DVA'
where   'FEEL_CODE_LIST' not in (select conf_code from conf);


-- update history;
\set mttb_name '\'feel\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;

