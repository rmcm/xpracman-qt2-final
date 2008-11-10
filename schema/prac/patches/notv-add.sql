-- Patch to add NOTV view
-- $Id$
--

\i schema/prac/notv-functions.sql
\i schema/prac/notv-crea.sql
\i meta-data/prac/notv-meta.sql
select create_metadata('notv');

delete from mtfn where mtfn_master_class = 'note' and mtfn_key = 'note_patn__sequence';
delete from mtrl where mtrl_other_class = 'note' and mtrl_join = 'note_patn__sequence';

\i dumpdir/prac/forms/notelist-notv-form.sql

-- register the patch
insert into mtpt(mtpt_name, mtpt_rcs_header) values('notv-add.sql', '$Id$');
