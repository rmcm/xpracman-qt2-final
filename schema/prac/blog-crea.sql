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


create table blog                       -- Blog Table for JAuditBlog
(
 blog_source text,                      -- Entity - source of blog (eg table or view)
 blog_key text,                         -- Primary key - eg id, sequence etc
 blog_category text,                    -- blog category
 blog_title text,                       -- title of blog entry
 blog_body text,                        -- blog body

 --
 -- standard headers
 --
 blog__sequence serial not null,                                -- Row-ID
 blog__timestamp timestamp default now() not null,              -- Timestamp
 blog__user_entry text default CURRENT_USER not null,           -- Entered by
 blog__status char default text 'N' not null,                   -- Record status
 --
 -- constraints
 --
 primary key(blog__sequence)
 );

grant select,insert,update, delete on blog to public;
grant select,insert,update on blog_blog__sequence_seq to public;

select create_metadata('blog');
update mtcl set mtcl_title = 'Blog Table for JAuditBlog' where mtcl_name = 'blog';

create or replace function fn_blog_delete()
returns trigger as '
  DECLARE
    BEGIN

    insert into blog(blog_source,
                     blog_key,
                     blog_category,
                     blog_title,
                     blog_body)
         values(''Blogger'',
                old.blog__sequence,
                ''system_blog'',
                ''Blogged a blog row: ''|| old.blog__sequence,
                ''<B>Blogger blogged:</B><BR>Testing<BR>'');

    return old;
    END;'
    LANGUAGE 'plpgsql';

create trigger tr_blog before delete
    on blog for each row
    execute procedure fn_blog_delete();

-- update history;
\set mttb_name '\'blog\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;

