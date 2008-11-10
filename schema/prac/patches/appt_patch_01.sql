-- Patch to add appointment module
-- $Id$
--
-- NOTE: you may need to dump existing data
--
--         gmake DBNAME=prac dumpclasses CLASSES="prov locn aptp apst evnt"   
--
-- and then restore with;
--
--         gmake DBNAME=prac loadclasses CLASSES="locn aptp apst evnt"
--

\i schema/prac/locn-crea.sql
\i schema/prac/locn-triggers.sql
\i meta-data/prac/locn-meta.sql
select create_metadata('locn');

\i schema/prac/aptp-crea.sql
\i schema/prac/aptp-triggers.sql
\i meta-data/prac/aptp-meta.sql
select create_metadata('aptp');

\i schema/prac/aptd-crea.sql
\i schema/prac/aptd-addconstraints.sql
\i meta-data/prac/aptd-meta.sql
select create_metadata('aptd');

\i schema/prac/apst-crea.sql
\i schema/prac/apst-triggers.sql
\i meta-data/prac/apst-meta.sql
select create_metadata('apst');

\i schema/prac/evnt-crea.sql
\i schema/prac/evnt-addconstraints.sql
\i meta-data/prac/evnt-meta.sql
select create_metadata('evnt');

\i schema/prac/evnv-functions.sql
\i schema/prac/evnv-crea.sql
\i meta-data/prac/evnv-meta.sql
select create_metadata('evnv');

alter table prov add column prov_colour text;
alter table prov alter column prov_colour set  default '#0000FF';
\i meta-data/prac/prov-meta.sql

-- reload metadata to resolve previous inconsistancies;
\i meta-data/prac/locn-meta.sql
\i meta-data/prac/aptp-meta.sql

-- register the patch
insert into mtpt(mtpt_name, mtpt_rcs_header) values('appt_patch_01.sql', '$Id$');
