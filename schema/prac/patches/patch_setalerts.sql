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
select disable_triggers();
UPDATE  patn
set     patn_amount_outstanding = (
                select  sum(invc_balance(invc__sequence))
                from    invc
                where   invc_patn__sequence = patn.patn__sequence );

UPDATE  patn
set     patn_last_service = (
                select  max(svpf_date_service)
                from    svpf
                where   svpf_patn__sequence = patn.patn__sequence );

UPDATE  patn
set     patn_last_visit = (
                select  max(evnt_starttime)
                from    evnt
                where   evnt_patn__sequence = patn.patn__sequence );

select enable_triggers();

-- register the patch
\set mtpt_name '\'patch_setalerts.sql\''
\set mtpt_rcs_header '\'$Id$\''

insert into mtpt(mtpt_name, mtpt_rcs_header) select :mtpt_name, :mtpt_rcs_header
        except select mtpt_name, mtpt_rcs_header from mtpt;

