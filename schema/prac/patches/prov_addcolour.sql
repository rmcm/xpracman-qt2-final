-- Patch to add provider colour code
-- $Id$

alter table prov add column prov_colour text;
\i meta-data/prac/prov-meta.sql

-- register the patch
insert into mtpt(mtpt_name, mtpt_rcs_header) values('prov_addcolour.sql', '$Id$');
