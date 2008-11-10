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

-- Create a virtual attribute for invc_balance

-- add svpf_invc_balance
delete from mtal where mtal_mtat_name = 'svpf_invc_balance';
delete from mtat where mtat_name = 'svpf_invc_balance';
insert into mtat(mtat_name, mtat_type, mtat_title, mtat_class_name, mtat_displen, mtat_browse_order, mtat_view_order, mtat_input_method) values('svpf_invc_balance', 'numeric(12,2)', 'Invoice Balance', 'svpf', 8, 120, 120, 'VIRTUAL=invc_balance(svpf_invc__sequence)');
update mtat set mtat_displen = 8 where mtat_name in ('svpf_invc__sequence', 'svpf_patn__sequence');
-- alert
insert into mtal(mtal_mtat_name,mtal_mtop_code,mtal_value,mtal_colour, mtal_priority)
        values('svpf_invc_balance',4, '0.00','#FF0000', 1);


-- update history;
\set mttb_name '\'svpf\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;
