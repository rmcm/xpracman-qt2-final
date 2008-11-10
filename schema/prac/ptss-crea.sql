--     $Id$
-- 
--   Copyright 2003 X=X Computer Software Trust
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

-- Create VIew "ptss" based on "svpf"
--

-- \i schema/DBNAME/ptss-functions.sql   -- Functions need to be defined before the VIEW

drop view ptss;
create view ptss                         -- Patient Services - Query view
  as select
  -- svpf
 to_char(svpf_date_service,'YYYY')      as ptss_year_service,
 svpf_serv_code                         as ptss_serv_code,
  -- invc
 invc_feet_code                         as ptss_feet_code,
  -- patn
 patn_hlfd_code                         as ptss_hlfd_code,
 count(*)                               as ptts_count,

--
-- standard headers
--
 1                                      as ptss__sequence,              -- Row-ID
 now()                                  as ptss__timestamp,             -- Timestamp
 CURRENT_USER                           as ptss__user_entry,            -- Entered by
 'V'::text                              as ptss__status                 -- Record status
 --
 from (svpf JOIN patn on (svpf_patn__sequence = patn__sequence)) as svpf_patn
        JOIN invc on (svpf_invc__sequence = invc__sequence)
 group by ptss_year_service,ptss_serv_code,ptss_feet_code,ptss_hlfd_code,
                ptss__sequence,ptss__timestamp,ptss__user_entry,ptss__status
;

grant select,insert,update on ptss to public;

delete from mtcl where mtcl_name = 'ptss';
delete from mtrl where mtrl_master_class = 'ptss';
select create_metadata('ptss');
update mtcl set mtcl_title = 'Patient Services Summary',
                mtcl_order_by = 'ptss_year_service,ptss_serv_code,ptss_feet_code,ptss_hlfd_code',
                mtcl_userlabel = 'ptss_year_service,ptss_serv_code,ptss_feet_code,ptss_hlfd_code',
                mtcl_query_limit = 400
         where mtcl_name = 'ptss';

-- insert into mtrl(mtrl_title, mtrl_master_class, mtrl_key, mtrl_other_class, mtrl_join)
--         values('Service Detail', 'ptss', 'ptss_year_service,ptss_serv_code,ptss_feet_code,ptss_hlfd_code',
--                  'ptsv', 'to_char(ptsv_date_service,\'year\'),ptsv_serv_code,ptsv_feet_code,ptsv_hlfd_code');

update mtat set mtat_input_method = 'RO' where mtat_class_name = 'ptss';
update mtat set mtat_input_method = 'SYSTEM' where mtat_name ~ '^ptss__.*';
update mtat set mtat_title = 'Year',      mtat_displen = 8  where mtat_name = 'ptss_year_service';
update mtat set mtat_title = 'Item',      mtat_displen = 8  where mtat_name = 'ptss_serv_code';
update mtat set mtat_title = 'Fee-type',  mtat_displen = 8  where mtat_name = 'ptss_feet_code';
update mtat set mtat_title = 'HealthFund',mtat_displen = 10 where mtat_name = 'ptss_hlfd_code';
update mtat set mtat_title = 'Count',     mtat_displen = 8  where mtat_name = 'ptss_count';

-- update history;
\set mttb_name '\'ptss\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;