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



-- Patch file

update mtat set mtat_title = 'Patient UR', mtat_input_method = '' where mtat_name = 'evnv_patn_flno';
update mtat set mtat_title = 'Postcode' where mtat_name = 'evnv_patn_postcode';
update mtat set mtat_title = 'Phone' where mtat_name = 'evnv_patn_phone';
update mtat set mtat_title = 'Referral Date' where mtat_name = 'evnv_patn_ref_date';
update mtat set mtat_title = 'Referral Period' where mtat_name = 'evnv_patn_ref_period';
update mtat set mtat_title = 'Last Visit' where mtat_name = 'evnv_patn_last_visit';
update mtat set mtat_title = 'Amount Owing' where mtat_name = 'evnv_patn_amount_outstanding';

\i schema/prac/evnv-crea.sql


-- register the patch
\set mtpt_name '\'patch-evnv_ur.sql\''
\set mtpt_rcs_header '\'$Id$\''

insert into mtpt(mtpt_name, mtpt_rcs_header) select :mtpt_name, :mtpt_rcs_header
        except select mtpt_name, mtpt_rcs_header from mtpt;

