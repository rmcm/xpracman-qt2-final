--  -*- sql -*-
--  
--    $Id$

--  Copyright 2005 X=X Computer Software Trust
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
-- View patient aged debt
-- ------------------------------------------------------------
drop view dbag;
create or replace view dbag
 as select 
 date(now() - (to_char(conf_integer('dbst_statement_age'),'9999') || ' days')::interval)
                                                                             as dbag_date,        -- aging date.
 patn__sequence                                                              as dbag_dbtr_code,
 patn_patient_name(patn__sequence)                                           as dbag_name,
 patn_address                                                                as dbag_address,     -- Street
 patn_suburb                                                                 as dbag_suburb,      -- Suburb
 patn_postcode                                                               as dbag_postcode,    -- Postcode
 patn_state                                                                  as dbag_state,       -- State
 patn_phone                                                                  as dbag_phone,       -- Phone
 patn_feet_code                                                              as dbag_feet_code,   -- fee type
 patn_patient_name(patn__sequence)                                           as dbag_patient_key, -- sort key
 ''::text                                                                    as dbag_last_statement,
 ''::text                                                                    as dbag_delay_statement,
 patn_total_balance(patn__sequence,conf_integer('dbst_statement_age'))       as dbag_total_balance,
 patn_period_balance(patn__sequence,0,conf_integer('dbst_statement_age'))    as dbag_balance_0,
 patn_period_balance(patn__sequence,30,conf_integer('dbst_statement_age'))   as dbag_balance_30,
 patn_period_balance(patn__sequence,60,conf_integer('dbst_statement_age'))   as dbag_balance_60,
 patn_period_balance(patn__sequence,90,conf_integer('dbst_statement_age'))   as dbag_balance_90,
 patn_period_balance(patn__sequence,120,conf_integer('dbst_statement_age'))  as dbag_balance_120,
 --
 -- standard headers
 --
 patn__sequence                                                    as dbag__sequence,   -- Row-ID
 patn__timestamp                                                   as dbag__timestamp,  -- Timestamp
 patn__user_entry                                                  as dbag__user_entry, -- Entered by
 patn__status                                                      as dbag__status      -- Record status
 --
 from  patn 
 where patn_total_balance(patn__sequence,conf_integer('dbst_statement_age')) > 0


;

grant select on dbag to public;

select create_metadata('dbag');

update mtcl set mtcl_title = 'Aged Patients', mtcl_order_by = 'order by dbag__sequence', mtcl_userlabel = 'dbag__sequence' where mtcl_name = 'dbag';
-- update mtat set mtat_title = '_@ATT_TITLE@_', mtat_displen = _@ATT_DISPLEN@_, mtat_view_order = _@ATT_VIEW_ORDER@_, mtat_input_method = '_@ATT_INPUT_METHOD@_'  where mtat_name = '_@ATT_NAME@_';
update mtat set mtat_title = 'ID', mtat_displen = 10, mtat_view_order = 10, mtat_input_method = 'RO'  where mtat_name = 'dbag_dbtr_code';
update mtat set mtat_title = 'FeeType', mtat_displen = 10, mtat_view_order = 12, mtat_input_method = 'RO'  where mtat_name = 'dbag_feet_code';
update mtat set mtat_title = 'Date', mtat_displen = 10, mtat_view_order = 14, mtat_input_method = 'RO'  where mtat_name = 'dbag_date';
update mtat set mtat_title = 'Name', mtat_displen = 10, mtat_view_order = 20, mtat_input_method = 'RO'  where mtat_name = 'dbag_name';
update mtat set mtat_title = 'Address', mtat_displen = 15, mtat_view_order = 30, mtat_input_method = 'RO'  where mtat_name = 'dbag_address';
update mtat set mtat_title = 'Suburb', mtat_displen = 10, mtat_view_order = 40, mtat_input_method = 'RO'  where mtat_name = 'dbag_suburb';
update mtat set mtat_title = 'Postcode', mtat_displen = 4, mtat_view_order = 50, mtat_input_method = 'RO'  where mtat_name = 'dbag_postcode';
update mtat set mtat_title = 'State', mtat_displen = 5, mtat_view_order = 60, mtat_input_method = 'RO'  where mtat_name = 'dbag_state';
update mtat set mtat_title = 'Phone', mtat_displen = 30, mtat_view_order = 70, mtat_input_method = 'RO'  where mtat_name = 'dbag_phone';
update mtat set mtat_title = 'Total Balance', mtat_displen = 10, mtat_view_order = 80, mtat_input_method = 'RO'  where mtat_name = 'dbag_total_balance';
update mtat set mtat_title = 'Last Statement', mtat_displen = 10, mtat_view_order = 82, mtat_input_method = 'RO'  where mtat_name = 'dbag_last_statement';
update mtat set mtat_title = 'Balance', mtat_displen = 10, mtat_view_order = 90, mtat_input_method = 'RO'  where mtat_name = 'dbag_balance_0';
update mtat set mtat_title = 'Delay Statement', mtat_displen = 5, mtat_view_order = 93, mtat_input_method = 'RO'  where mtat_name = 'dbag_delay_statement';
update mtat set mtat_title = 'Patient Key', mtat_displen = 10, mtat_view_order = 96, mtat_input_method = 'RO'  where mtat_name = 'dbag_patient_key';
update mtat set mtat_title = 'Balance 30', mtat_displen = 10, mtat_view_order = 100, mtat_input_method = 'RO'  where mtat_name = 'dbag_balance_30';
update mtat set mtat_title = 'Balance 60', mtat_displen = 10, mtat_view_order = 110, mtat_input_method = 'RO'  where mtat_name = 'dbag_balance_60';
update mtat set mtat_title = 'Balance 90', mtat_displen = 10, mtat_view_order = 120, mtat_input_method = 'RO'  where mtat_name = 'dbag_balance_90';
update mtat set mtat_title = 'Balance 120', mtat_displen = 10, mtat_view_order = 130, mtat_input_method = 'RO'  where mtat_name = 'dbag_balance_120';
update mtat set mtat_title = 'Sequence', mtat_displen = 10, mtat_view_order = 140, mtat_input_method = 'SYSTEM'  where mtat_name = 'dbag__sequence';
update mtat set mtat_title = 'Timestamp', mtat_displen = 47, mtat_view_order = 150, mtat_input_method = 'SYSTEM'  where mtat_name = 'dbag__timestamp';
update mtat set mtat_title = 'User Entry', mtat_displen = 30, mtat_view_order = 160, mtat_input_method = 'SYSTEM'  where mtat_name = 'dbag__user_entry';
update mtat set mtat_title = 'Status', mtat_displen = 1, mtat_view_order = 170, mtat_input_method = 'SYSTEM'  where mtat_name = 'dbag__status';


-- make the child of AGDI
delete from mtrl where mtrl_other_class = 'dbag';
insert into mtrl(mtrl_title, mtrl_master_class, mtrl_key, mtrl_other_class, mtrl_join) values('Patients', 'agdt', 'agdt_feet_code', 'dbag', 'dbag_feet_code');

-- cleanup the agdt parent
update mtcl set mtcl_query_limit = 0 where mtcl_name = 'agdt';
update mtat set mtat_title = 'Invoice Count' where mtat_name = 'agdt_count';

-- update history;

\set mttb_name '\'dbag\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;
