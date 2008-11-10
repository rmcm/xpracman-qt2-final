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

begin;
insert into empl(
        empl_code,
        empl_name,
        empl_address,
        empl_suburb,
        empl_postcode,
        empl_state)
select  rfdr_code,
        rfdr_name,
        rfdr_street,
        rfdr_suburb,
        rfdr_postcode,
        rfdr_state
from    rfdr
where   rfdr_code in
        (select patn_rfdr_code from patn where patn_feet_code = '12')
and     rfdr_code not in (select empl_code from empl)
;

select disable_triggers();

update  patn
set     patn_empl_code = patn_rfdr_code
where   patn_feet_code = '12'
and     patn_empl_code =  '-';

update  invc
set     invc_empl_code = invc_rfdr_code
where   invc_feet_code = '12'
and     invc_empl_code =  '-';


select enable_triggers();

update  fmdt
set     fmdt_text = 'svpv_empl_name'
where   fmdt_form_code = 'invoice-svpv_12'
and     fmdt_text = 'svpv_rfdr_name';

commit;
