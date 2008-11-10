--     $Id$
-- 
--   Copyright 2007 X=X Computer Software Trust
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



--SELECT dbst_dbtr_code,dbst_total_balance,dbst_balance_0,dbst_balance_30,dbst_group,dbst_balance_60,dbst_last_statement,dbst_balance_90,dbst_first_statement,dbst_last_invoice,dbst_balance_120,dbst_delay_statement,dbst_invc__sequence,dbst_amount,dbst_amount_outstanding,dbst_paid,dbst_balance,dbst_period,dbst_date_printed,dbst_date_reprint 
--from dbst 
--where  dbst_total_balance != 0.00::numeric
--and    (dbst_last_statement is null 
--       or dbst_last_statement < dbst_date - coalesce(dbst_delay_statement,'30 days'::interval) )
--and    ( (dbst_total_balance > dbst_balance_0 + dbst_balance_30) 
--         or (dbst_last_invoice < dbst_date - coalesce(dbst_first_statement,'40 days'::interval)))
--and    dbst_balance != 0 
--order by dbst_patient_key, dbst_date_printed; 
--
--select dbst_last_statement is null, dbst_last_statement < dbst_date - coalesce(dbst_delay_statement,'30 days'::interval), (dbst_last_statement is null or dbst_last_statement < dbst_date - coalesce(dbst_delay_statement,'30 days'::interval)) from dbst where dbst_last_statement is null and dbst_balance != 0 order by dbst_last_invoice; 
--select dbst_total_balance, dbst_balance_0, dbst_balance_30, dbst_last_invoice, dbst_date - coalesce(dbst_first_statement,'40 days'::interval), (dbst_total_balance > dbst_balance_0 + dbst_balance_30) or (dbst_last_invoice < dbst_date - coalesce(dbst_first_statement,'40 days'::interval))  from dbst where dbst_last_statement is null and dbst_balance != 0 order by dbst_last_invoice; 
--
--
---- Create VIew "dbsc" based on "dbst"
--

-- \i schema/DBNAME/dbsc-functions.sql   -- Functions need to be defined before the VIEW

create or replace view dbsc                         -- Check if statement due
  as select
  
 dbst_dbtr_code                         as dbsc_dbtr_code,
 dbst_total_balance != 0.00::numeric    as dbsc_statement_balance,
 dbst_last_statement is null            as dbsc_first_statement,
 (dbst_last_statement < dbst_date - coalesce(dbst_delay_statement,'30 days'::interval))
                                        as dbsc_statement_due,
 (dbst_total_balance > dbst_balance_0 + dbst_balance_30)
                                        as dbsc_over_30_days,
 (dbst_last_invoice < dbst_date - coalesce(dbst_first_statement,'40 days'::interval))
                                        as dbsc_stale_invoice,
 (dbst_balance != 0)                    as dbsc_invoice_balance,

 --
-- standard headers
--
 dbst__sequence                         as dbsc__sequence,              -- Row-ID
 dbst__timestamp                        as dbsc__timestamp,             -- Timestamp
 dbst__user_entry                       as dbsc__user_entry,            -- Entered by
 dbst__status                           as dbsc__status                 -- Record status
 --
 from dbst
;

-- ------------------------------------------------------------
-- PERMISSIONS
-- ------------------------------------------------------------
grant select,insert,update on dbsc to public;

-- ------------------------------------------------------------
-- METADATA
-- ------------------------------------------------------------
-- add the default metadata
select create_metadata('dbsc');

update mtcl set mtcl_title = 'Check statement status', mtcl_order_by = 'order by dbsc__sequence' where mtcl_name = 'dbsc';
--
-- /home/source/bin/makeDBSubstitute.sh module database table
-- update mtat set mtat_title = '_@ATT_TITLE@_', mtat_displen = _@ATT_DISPLEN@_, mtat_input_method = '_@ATT_INPUT_METHOD@_', mtat_view_order = _@ATT_VIEW_ORDER@_ where mtat_name = '_@ATT_NAME@_';

-- update history;
\set mttb_name '\'dbsc\''
\set mttb_rcs_header '\'$Id: my-macros.dm,v 1.37 2007/02/19 08:18:43 source Exp $\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;
