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
-- Function: 
-- ------------------------------------------------------------

create or replace function fn_feet_invoiced(text,text)
returns numeric
as 'DECLARE

    a_feet_code ALIAS for $1;
    a_month     ALIAS for $2;
    total       numeric;

    BEGIN

    select sum(invc_amount + invc_gst_amount)
    into   total
    from   invc
    where  invc_feet_code = a_feet_code
    and    to_char(invc_date_printed,''YYYY-MM'') = a_month;
    
    return total;
    END;'
  LANGUAGE 'plpgsql';


-- Create VIew "rnpv" based on "svpf"
--

drop view rnpv;
create view rnpv                         -- Report - New patients
  as select
  count(*)                              as rnpv_count,
  to_char(svpf_date_service,'YYYY-MM')  as rnpv_month,
  patn_feet_code                        as rnpv_feet_code,  
  feet_desc                             as rnpv_feet_desc,
  fn_feet_invoiced(patn_feet_code, to_char(svpf_date_service,'YYYY-MM'))
                                        as rnpv_invoiced,

--
-- standard headers
--
(feet__sequence||to_char(svpf_date_service,'YYYYMM'))::int
                                        as rnpv__sequence,              -- Row-ID
 now()                                  as rnpv__timestamp,             -- Timestamp
 feet__user_entry                       as rnpv__user_entry,            -- Entered by
 feet__status                           as rnpv__status                 -- Record status
 --
 from (feet LEFT JOIN patn ON (patn.patn_feet_code = feet.feet_code))
        LEFT JOIN svpf ON (patn.patn__sequence = svpf.svpf_patn__sequence) 
 where svpf__sequence = (select min(svpf__sequence)
                         from   svpf
                         where  svpf_patn__sequence = patn.patn__sequence
                         and    svpf_serv_code = '104')
 GROUP BY to_char(svpf_date_service,'YYYY-MM'),
          patn_feet_code,
          feet_desc,
          fn_feet_invoiced(patn_feet_code, to_char(svpf_date_service,'YYYY-MM')),
          (feet__sequence||to_char(svpf_date_service,'YYYYMM'))::int,
          now(),
          feet__user_entry,
          feet__status
;

grant select,insert,update on rnpv to public;

-- Rules

-- Insert Rule
-- The insert is more flexibly handled by a function;
--    eg rnpv_insert_row() in rnpv-functions.sql

-- Update rule
-- drop rule rl_rnpv_update;
-- create rule rl_rnpv_update as on update
--        to rnpv
--        do instead
--        (
--         update svpf
--         set    svpf_  = new.rnpv_
--         where  svpf_  = new.rnpv_
--        );
-- 
-- -- Delete rule
-- drop rule rl_rnpv_delete;
-- create rule rl_rnpv_delete as on delete
--        to rnpv
--        do instead
--        (
--         delete from svpf
--         where  svpf_  = old.rnpv_
--        );
-- 

-- add the default metadata
select create_metadata('rnpv');
update mtcl set mtcl_title = 'Report - New patients', mtcl_order_by = 'order by rnpv_month, rnpv_feet_code' where mtcl_name = 'rnpv';
-- update mtat set mtat_title = '_@ATT_TITLE@_', mtat_displen = _@ATT_DISPLEN@_, mtat_view_order = _@ATT_VIEW_ORDER@_ where mtat_name = '_@ATT_NAME@_';
update mtat set mtat_title = 'Month', mtat_displen = 10, mtat_view_order = 20 where mtat_name = 'rnpv_month';
update mtat set mtat_title = 'Fee Type', mtat_displen = 10, mtat_view_order = 40 where mtat_name = 'rnpv_feet_code';
update mtat set mtat_title = 'Description', mtat_displen = 30, mtat_view_order = 50 where mtat_name = 'rnpv_feet_desc';
update mtat set mtat_title = 'Invoiced', mtat_displen = 10, mtat_view_order = 100 where mtat_name = 'rnpv_invoiced';
update mtat set mtat_title = 'Count', mtat_displen = 10, mtat_view_order = 110 where mtat_name = 'rnpv_count';
update mtat set mtat_title = 'Sequence', mtat_displen = 30, mtat_view_order = 60 where mtat_name = 'rnpv__sequence';
update mtat set mtat_title = 'Timestamp', mtat_displen = 30, mtat_view_order = 70 where mtat_name = 'rnpv__timestamp';
update mtat set mtat_title = 'User Entry', mtat_displen = 30, mtat_view_order = 80 where mtat_name = 'rnpv__user_entry';
update mtat set mtat_title = 'Status', mtat_displen = 1, mtat_view_order = 90 where mtat_name = 'rnpv__status';

-- update history;
\set mttb_name '\'rnpv\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;

