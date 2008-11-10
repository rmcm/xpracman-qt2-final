-- Patch to allow more felxible debtor addresses
-- $Id$

\i schema/prac/dbtr-functions.sql
\i schema/prac/dbst-crea.sql
select create_metadata('dbst');
\i schema/prac/svpv-crea.sql
select create_metadata('svpv');
\i schema/prac/dbag-crea.sql

-- might also need to manually change the address layout

update fmdt set fmdt_text = 'svpv_dbtr_full_address', fmdt_height=72, fmdt_vt_align=1  where fmdt_form_code ~ '^invoice-svpv' and fmdt_text = 'svpv_dbtr_name';
delete from fmdt where fmdt_form_code ~ '^invoice-svpv' and fmdt_text in ('svpv_dbtr_address','svpv_dbtr_suburb','svpv_dbtr_state');

update fmdt set fmdt_text = 'dbst_full_address', fmdt_height=60, fmdt_vt_align=1 where fmdt_form_code ~ '^dbstatement' and fmdt_text = 'dbst_name';
delete from fmdt where fmdt_form_code ~ '^dbstatement' and fmdt_text in ('dbst_address','dbst_suburb','dbst_state');


-- register the patch
insert into mtpt(mtpt_name, mtpt_rcs_header) values('dbtraddress.sql', '$Id$');
