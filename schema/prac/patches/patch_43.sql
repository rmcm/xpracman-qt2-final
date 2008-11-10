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

-- Master patch to update from 42 to 43

\i schema/marvin/create_metadata.sql
\i schema/prac/patn_more_phones.sql
\i schema/prac/patn_last_service.sql
\i schema/prac/patn_last_visit.sql
\i schema/prac/patn_last_note.sql
\i schema/prac/patn-functions.sql
\i schema/prac/empl_notes_phone.sql
\i schema/prac/patches/patch-add_crednotes.sql 
\i schema/prac/patches/patch-evnv_ur.sql 
\i schema/prac/patches/patch-addcrlt.sql 
\i schema/prac/patches/patch-fixcredsummary.sql 
\i schema/prac/patches/patch-addsclt.sql 
\i schema/prac/patches/patch-addrealdate.sql 
\i schema/prac/patches/patch_setalerts.sql 
\i schema/prac/patches/postcode_extend.sql 
\i schema/prac/patches/patch_aptp_01.sql 
\i schema/prac/patches/extend-invoiceheaders.sql 
\i schema/prac/patches/extend-evnt.sql 
\i schema/prac/patches/patch-fixcredsummary.sql 

\i schema/prac/aptp-triggers.sql
\i schema/prac/bkdp-triggers.sql
\i schema/prac/cred-triggers.sql
\i schema/prac/crev-functions.sql
\i schema/prac/dbtr-functions.sql
\i schema/prac/eftr-triggers.sql
\i schema/prac/patn_amount_outstanding.sql
\i schema/prac/invc-triggers.sql
\i schema/prac/pate-crea.sql
\i schema/prac/paym-triggers.sql
\i schema/prac/svpf-triggers.sql
\i schema/prac/svpf-functions.sql
\i schema/prac/prov-functions.sql
\i schema/prac/svpv-crea.sql
\i schema/prac/evnv-crea.sql
\i schema/prac/patv-crea.sql

grant delete on svlt to public;
grant delete on sclt to public;
grant delete on svsm to public;
grant delete on crsm to public;
select disable_triggers();
update aptp set aptp_duration=15 where aptp_duration is null;
select enable_triggers();
alter table patn alter column patn_hlfd_code set default '-';
grant delete on crev to public;
-- \i schema/marvin/mtau-crea.sql
alter table aptd alter column aptd_dayofweek drop default;
