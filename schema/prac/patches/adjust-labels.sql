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


-- adjust label formats

update form set form_mg_top = 44 where form_code ~ '^label_patient' ;
update fmdt set fmdt_y_coord = (fmdt_y_coord/120)*108 +(fmdt_y_coord%120)+8 where fmdt_form_code ~ '^label_patient' ;
update fmdt set fmdt_y_coord = fmdt_y_coord - 8,fmdt_height = 20, fmdt_ft_size = 14 where fmdt_text ~ 'patv_psnam' and fmdt_form_code = 'label_patient_base' ;
update fmdt set fmdt_y_coord = fmdt_y_coord - 8,fmdt_height = 20 where fmdt_text ~ 'patv_feet_code' and fmdt_form_code = 'label_patient_base' ;
