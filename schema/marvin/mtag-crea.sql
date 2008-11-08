--     $Id: mtag-crea.sql 2077 2004-02-01 05:40:23Z source $
-- 
--   Copyright 2003 X=X Computer Software Trust
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



drop table mtag;
drop sequence mtag_mtag__sequence_seq;
create table mtag                       -- Application groups
(
 mtag_name text default 'new' not null,
 mtag_title text default 'new',
 mtag_parent text references mtag ON DELETE set null ON UPDATE cascade,
 mtag_access int default 31 not null,
 mtag_list_order text default 'Z-LAST' not null,
 --
 -- standard headers
 --
 mtag__sequence serial not null,                                -- Row-ID
 mtag__timestamp timestamp default now() not null,              -- Timestamp
 mtag__user_entry text default CURRENT_USER not null,           -- Entered by
 mtag__status char default text 'N' not null,                   -- Record status
 --
 -- constraints
 --
 primary key(mtag_name)
 );

grant select,insert,update on mtag to public;
grant select,insert,update on mtag_mtag__sequence_seq to public;

-- alter mtcl to add contraint
update mtcl set mtcl_group = null;
alter table mtcl
        add constraint mtcl_fk_mtag
            foreign key (mtcl_group)
            references mtag
            on update cascade
            on delete set null
            initially deferred;

-- Scrounge some records.
insert into mtag(mtag_name, mtag_title) values('METADATA', 'Meta Data');
insert into mtag(mtag_name)
        select  mtcl_group
        from    mtcl
        where   mtcl_group is not null
        except
        select  mtag_name
        from    mtag;
update mtag set mtag_title = mtag_name where mtag_title is null;

-- cleanup the metadata
delete from mtcl where mtcl_name = 'mtag';

select create_metadata('mtag');
update mtcl set mtcl_title = 'Application Groups',
                mtcl_group = 'METADATA',
                mtcl_order_by = 'order by mtag_list_order asc'  where mtcl_name = 'mtag';
update mtcl set mtcl_group = 'METADATA' where mtcl_name ~ '^mt..$';
update mtat set mtat_displen = 10, mtat_title = 'Name' where mtat_name = 'mtag_name';
update mtat set mtat_displen = 20, mtat_title = 'Title' where mtat_name = 'mtag_title';
update mtat set mtat_displen = 10, mtat_title = 'Parent Menu'  where mtat_name = 'mtag_parent';
update mtat set mtat_displen = 5, mtat_title = 'Access'  where mtat_name = 'mtag_access';
update mtat set mtat_displen = 10, mtat_title = 'List Order'  where mtat_name = 'mtag_list_order';

-- update history;
\set mttb_name '\'mtag\''
\set mttb_rcs_header '\'$Id: mtag-crea.sql 2077 2004-02-01 05:40:23Z source $\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;

-- The menu items
--
create table mtad                       -- Application Details
(
 mtad_title text,
 mtad_icon text,
 mtad_parent text references mtag ON DELETE set null ON UPDATE cascade,
 mtad_class text references mtcl ON DELETE set null ON UPDATE cascade,
 mtad_access int default 31 not null,
 mtad_list_order text default 'Z-LAST' not null,
 --
 -- standard headers
 --
 mtad__sequence serial not null,                                -- Row-ID
 mtad__timestamp timestamp default now() not null,              -- Timestamp
 mtad__user_entry text default CURRENT_USER not null,           -- Entered by
 mtad__status char default text 'N' not null,                   -- Record status
 --
 -- constraints
 --
 primary key(mtad__sequence)
 );

grant select,insert,update on mtad to public;
grant select,insert,update on mtad_mtad__sequence_seq to public;

-- cleanup the metadata
delete from mtcl where mtcl_name = 'mtad';
select create_metadata('mtad');

update mtcl set mtcl_title = 'Application Detail',
                mtcl_group = 'METADATA',
                mtcl_order_by = 'order by mtad_list_order asc'  where mtcl_name = 'mtad';
update mtcl set mtcl_group = 'METADATA' where mtcl_name ~ '^mt..$';
update mtat set mtat_displen = 20 where mtat_name = 'mtad_title';
update mtat set mtat_displen = 10 where mtat_name = 'mtad_icon';
update mtat set mtat_displen = 10 where mtat_name = 'mtad_parent';
update mtat set mtat_displen = 10 where mtat_name = 'mtad_class';
update mtat set mtat_displen = 10 where mtat_name = 'mtad_access';
update mtat set mtat_displen = 10 where mtat_name = 'mtad_list_order';

delete from mtfn where mtfn_master_class = 'mtad' or mtfn_other_class = 'mtad';
delete from mtrl where mtrl_master_class = 'mtad' or mtrl_other_class = 'mtad';
delete from mtfn where mtfn_master_class = 'mtag' or mtfn_other_class = 'mtag';
delete from mtrl where mtrl_master_class = 'mtag' or mtrl_other_class = 'mtag';
select create_fk_links('mtad');
select create_rl_links('mtad');
select create_fk_links('mtag');
select create_rl_links('mtag');

update mtfn set mtfn_title = 'Menus' where mtfn_other_class = 'mtag';
update mtfn set mtfn_title = 'Menu Items' where mtfn_other_class = 'mtad';
update mtrl set mtrl_title = 'Menu Items' where mtrl_other_class = 'mtad';
update mtrl set mtrl_title = 'Menus' where mtrl_other_class = 'mtag';
update mtrl set mtrl_title = 'Classes' where mtrl_other_class = 'mtcl';


-- Scrounge some records.
insert into mtad(mtad_parent, mtad_class, mtad_title)
        select  mtcl_group, mtcl_name, mtcl_title
        from    mtcl
        where   mtcl_group = 'METADATA'
        except
        select  mtad_parent, mtad_class, mtad_title
        from    mtad;

update mtag set mtag_access = '32' where mtag_name = 'METADATA';
--
-- update history;
\set mttb_name '\'mtad\''
\set mttb_rcs_header '\'$Id: mtag-crea.sql 2077 2004-02-01 05:40:23Z source $\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;

