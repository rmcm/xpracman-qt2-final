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


-- Convert PRAC-4.1 DB to PRAC-4.2

-- new meta tables
\i schema/marvin/create_metadata.sql
\i schema/marvin/mtop-crea.sql
\i schema/marvin/mtal-crea.sql
\i schema/marvin/mtag-crea.sql
\i schema/marvin/mttk-crea.sql
\i schema/marvin/mtau-crea.sql

-- cleanup metameta
update mvac_database set perms = '2';
update mvac_user set perms = '3';
update mvac_user set perms = '127' where username = 'source';
-- 
-- -- new prac tables/views
        -- POSTCODE
\i schema/prac/pcde-crea.sql
update mtat set mtat_input_method = 'COPYTEXT=pcde;pcde_postcode;pcde_locality;patn_suburb' where mtat_name = 'patn_postcode';
update mtat set mtat_input_method = 'COPYTEXT=pcde;pcde_locality;pcde_postcode;patn_postcode' where mtat_name = 'patn_suburb';
update mtat set mtat_input_method = 'COPYTEXT=pcde;pcde_postcode;pcde_locality;evnv_patn_suburb' where mtat_name = 'evnv_patn_postcode';
update mtat set mtat_input_method = 'COPYTEXT=pcde;pcde_locality;pcde_postcode;evnv_patn_postcode' where mtat_name = 'evnv_patn_suburb';
        -- 
-- not ready yet \i schema/prac/ptsv-crea.sql
\i schema/prac/ptss-crea.sql
\i schema/prac/addr-crea.sql

-- updated tables/views
\i schema/prac/patn_last_visit.sql
\i schema/prac/patn_last_service.sql
\i schema/prac/patn_amount_outstanding.sql
\i schema/prac/evnv-crea.sql
\i schema/prac/svpf_invc_balance.sql

-- add patn_ref_expired (virtual)
\i schema/prac/patn_ref_expired.sql

-- make sure new attributes are RO.
update mtat set mtat_input_method = 'RO'
        where mtat_name in ('patn_last_visit','patn_amount_outstanding','evnv_patn_last_visit','evnv_patn_amount_outstanding','patn_last_service');

-- DBTR first-statement interval
alter table dbtr add column dbtr_first_statement interval;
alter table dbtr alter column dbtr_first_statement set default '40 days';
select create_metadata('dbtr');

-- Allow banked payments to be modified by admin-office
drop view dbst;
drop view svpv;
drop view dbag;
drop view svlt;
\i schema/marvin/mvac_label-crea.sql
update mtag set mtag_access = '64' where mtag_name = 'METADATA';

\i schema/prac/dbtr-functions.sql
\i schema/prac/cred-functions.sql

\i schema/prac/paym-triggers.sql
\i schema/prac/bkdp-triggers.sql
\i schema/prac/cred-triggers.sql
\i schema/prac/invc-triggers.sql
\i schema/prac/svpf-triggers.sql

\i schema/prac/dbst-crea.sql
\i schema/prac/svpv-crea.sql
\i schema/prac/dbag-crea.sql
\i schema/prac/svlt-crea.sql

-- 

-- some default alerts

-- default userlabels
\i schema/prac/patches/mtcl-userlabels.sql


-- update the new patn triggered attributes

update  patn
set     patn_amount_outstanding = (
            select  sum(invc_balance(invc__sequence))
            from    invc
            where   invc_patn__sequence = patn.patn__sequence );

update  dbtr
set     dbtr_amount_outstanding = (
            select  sum(invc_balance(invc__sequence))
            from    invc
            where   invc_dbtr_code = dbtr.dbtr_code );

update  patn
set     patn_last_service = (
                select  max(svpf_date_service)
                from    svpf
                where   svpf_patn__sequence =patn.patn__sequence );

update  patn
set     patn_last_visit = (
                select  max(evnt_starttime)
                from    evnt
                where   evnt_patn__sequence = patn.patn__sequence );



-- register the patch
\set mtpt_name '\'patch-41to42.sql\''
\set mtpt_rcs_header '\'$Id$\''

insert into mtpt(mtpt_name, mtpt_rcs_header) select :mtpt_name, :mtpt_rcs_header
        except select mtpt_name, mtpt_rcs_header from mtpt;
