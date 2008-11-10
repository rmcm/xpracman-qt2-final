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

-- original set.

update mtat set mtat_input_method = 'COPYTEXT=pcde;pcde_postcode;pcde_locality;patn_suburb' where mtat_name = 'patn_postcode';
update mtat set mtat_input_method = 'COPYTEXT=pcde;pcde_locality;pcde_postcode;patn_postcode' where mtat_name = 'patn_suburb';
update mtat set mtat_input_method = 'COPYTEXT=pcde;pcde_postcode;pcde_locality;evnv_patn_suburb' where mtat_name = 'evnv_patn_postcode';
update mtat set mtat_input_method = 'COPYTEXT=pcde;pcde_locality;pcde_postcode;evnv_patn_postcode' where mtat_name = 'evnv_patn_suburb';

-- remaining

update mtat set mtat_input_method = 'COPYTEXT=pcde;pcde_postcode;pcde_locality;rfdr_suburb' where mtat_name = 'rfdr_postcode';
update mtat set mtat_input_method = 'COPYTEXT=pcde;pcde_locality;pcde_postcode;rfdr_postcode' where mtat_name = 'rfdr_suburb';
update mtat set mtat_input_method = 'COPYTEXT=pcde;pcde_postcode;pcde_locality;bank_suburb' where mtat_name = 'bank_postcode';
update mtat set mtat_input_method = 'COPYTEXT=pcde;pcde_locality;pcde_postcode;bank_postcode' where mtat_name = 'bank_suburb';
update mtat set mtat_input_method = 'COPYTEXT=pcde;pcde_postcode;pcde_locality;dbtr_suburb' where mtat_name = 'dbtr_postcode';
update mtat set mtat_input_method = 'COPYTEXT=pcde;pcde_locality;pcde_postcode;dbtr_postcode' where mtat_name = 'dbtr_suburb';
update mtat set mtat_input_method = 'COPYTEXT=pcde;pcde_postcode;pcde_locality;empl_suburb' where mtat_name = 'empl_postcode';
update mtat set mtat_input_method = 'COPYTEXT=pcde;pcde_locality;pcde_postcode;empl_postcode' where mtat_name = 'empl_suburb';
update mtat set mtat_input_method = 'COPYTEXT=pcde;pcde_postcode;pcde_locality;inst_suburb' where mtat_name = 'inst_postc';
update mtat set mtat_input_method = 'COPYTEXT=pcde;pcde_locality;pcde_postcode;inst_postc' where mtat_name = 'inst_suburb';
update mtat set mtat_input_method = 'COPYTEXT=pcde;pcde_postcode;pcde_locality;prov_suburb' where mtat_name = 'prov_postcode';
update mtat set mtat_input_method = 'COPYTEXT=pcde;pcde_locality;pcde_postcode;prov_postcode' where mtat_name = 'prov_suburb';
