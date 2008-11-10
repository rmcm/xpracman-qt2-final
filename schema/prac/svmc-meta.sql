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



-- Create MetaData for Object "svmc"
--

-- modify the default metadata

update mtcl set mtcl_title = 'Medicare Claim Results against service', mtcl_order_by = 'order by svmc__sequence' where mtcl_name = 'svmc';

-- pass the following to makeDBSubstitute.sh MODULE DATABASE OBJECT
-- (MODULE is not used here - pass a dummy argument)
-- update mtat set mtat_title = '_@ATT_TITLE@_', mtat_displen = _@ATT_DISPLEN@_, mtat_view_order = _@ATT_VIEW_ORDER@_ where mtat_name = '_@ATT_NAME@_';

update mtat set mtat_title = 'Service ID', mtat_displen = 30, mtat_view_order = 10 where mtat_name = 'svmc_svpf__sequence';
update mtat set mtat_title = 'RSN Code', mtat_displen = 30, mtat_view_order = 20 where mtat_name = 'svmc_rsn_code';
update mtat set mtat_title = 'Benefit Amount', mtat_displen = 786434, mtat_view_order = 30 where mtat_name = 'svmc_benefit_amount';
update mtat set mtat_title = 'ID', mtat_displen = 30, mtat_view_order = 40 where mtat_name = 'svmc__sequence';
update mtat set mtat_title = 'Timestamp', mtat_displen = 30, mtat_view_order = 50 where mtat_name = 'svmc__timestamp';
update mtat set mtat_title = 'User Entry', mtat_displen = 30, mtat_view_order = 60 where mtat_name = 'svmc__user_entry';
update mtat set mtat_title = 'Status', mtat_displen = 1, mtat_view_order = 70 where mtat_name = 'svmc__status';

-- update history;
\set mttb_name '\'svmc\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;

