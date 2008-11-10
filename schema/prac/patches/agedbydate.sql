-- Patch to allow aged debtor report at specified date.
-- $Id$

\i schema/prac/invc-functions.sql
\i schema/prac/dbtr-functions.sql
\i schema/prac/dbag-crea.sql
select create_metadata('dbag');
\i schema/prac/svpv-crea.sql
\i schema/prac/dbst-crea.sql
\i schema/prac/crev-crea.sql
\i schema/prac/crep-crea.sql
\i schema/prac/agdi-crea.sql
\i schema/prac/agdd-crea.sql
\i schema/prac/agdp-crea.sql
\i schema/prac/agdt-crea.sql
-- might also need to manually change the report date attribute
update fmdt set fmdt_field_type = 1 where fmdt_form_code ~ 'dbag' and fmdt_text = 'dbag_date';

-- register the patch
insert into mtpt(mtpt_name, mtpt_rcs_header) values('agedbydate.sql', '$Id$');
