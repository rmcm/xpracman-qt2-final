-- Patch to fix but in batch (unprinted) invoice printing
--
-- 1. load new view
\i schema/prac/svpv-crea.sql

-- 2. Add metadata
select create_metadata('svpv');

-- 3. Patch invoice form

update fmdt set fmdt_text = 'svpv_date_printed' where fmdt_text = 'svpv_invc_date_printed';
update fmdt set fmdt_text = 'svpv_date_reprint' where fmdt_text = 'svpv_invc_date_reprint';
