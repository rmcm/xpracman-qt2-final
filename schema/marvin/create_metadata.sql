-- -*-sql-*-
--      $Id: create_metadata.sql 3041 2006-09-18 22:43:29Z source $ 
--
--    Copyright 2000 X=X Computer Software Trust 
--                  Kangaroo Ground Australia 3097 
--
--
--   This is free software; you can redistribute it and/or modify 
--   it under the terms of the GNU General Public License published by 
--   the Free Software Foundation; either version 2, or (at your option) 
--   any later version. 
--
--   This software is distributed in the hope that it will be useful, 
--   but WITHOUT ANY WARRANTY; without even the implied warranty of 
--   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
--   GNU General Public License for more details. 
--
--   You should have received a copy of the GNU General Public License 
--   along with this software; see the file COPYING.  If not, write to 
--   the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA. 
--
--   Report problems and direct all questions to: 
--
--       Rex McMaster, rmcm@compsoft.com.au 
--

--
-- PL function to create meta-date from pg system tables
--

create or replace function create_metadata(text)
returns bool as $$ 

  DECLARE
    classname alias for $1;
    mtcl_row mtcl%ROWTYPE;
    pg_class_oid pg_class.oid%TYPE;
    pg_class_relname pg_class.relname%TYPE;
    mtcl_pk mtcl.mtcl_primary%TYPE;
    mtat_row RECORD;
    x_counter integer;
    -- regexp mtat.mtat_input_method%TYPE;

    BEGIN

        -- get class system metadata
        select oid,
               relname
        into   pg_class_oid,
               pg_class_relname
        from   pg_class
        where  relname = classname;
        if not found then
           RAISE NOTICE 'Unable to locate <%> in system catalogues', classname;
           return false;
        end if;


        -- Check for existing mtcl record
        select into mtcl_row *
        from   mtcl
        where  mtcl_name = classname;
        if (not found) then
          -- No existing MTCL record, so create one.
          
          -- get primary key
          select a.attname
          into   mtcl_pk
          from   pg_index i,
                 pg_attribute a
          where  i.indrelid = pg_class_oid
          and    i.indkey[0] = a.attnum
          and    a.attrelid = pg_class_oid
          and    i.indisprimary = 't';
          if not found then
             mtcl_pk = classname||'__sequence';
          end if;
          
          -- insert a mtcl record
          
          insert into mtcl(
                      mtcl_access,
                      mtcl_name,
                      mtcl_title,
                      mtcl_group,
                      mtcl_matrix_order,
                      mtcl_order_by,
                      mtcl_group_by,
                      mtcl_primary,
                      mtcl_userkey,
                      mtcl_userlabel,
                      mtcl_query_limit,
                      mtcl_query_offset,
                      mtcl_extras)
                  values(
                      '31',
                      classname,
                      classname,
                      NULL,
                      10000,
                      'order by '||classname||'__sequence',
                      NULL,
                      mtcl_pk,
                      mtcl_pk,
                      mtcl_pk,
                      200,
                      0,
                      NULL);
        end if;

        -- insert attribute metadata
        -- regexp := classname||'__';
        x_counter = 0;
        FOR mtat_row IN
          select  a.attnum,
                  a.attname,
                  initcap(translate(substr(a.attname, position('_' in a.attname)+1), '_', ' '))
                                                    as att_title,
                  t.typname,
                  (case when a.atttypmod > 0 then
                              a.atttypmod-4 
                    else 30
                   end) as mtat_length,
                   (case when a.attname ~ '^....__' then
                      'SYSTEM'::text
                    else
                      '10'::text
                    end) as mtat_input_method
          from     pg_attribute a,
                   pg_type t
          where    a.attname not in (select mtat_name from mtat)
          and      a.attrelid = pg_class_oid
          and      a.attnum > 0
          and      a.atttypid = t.oid
          order by attnum
        LOOP
          insert into mtat(
                      mtat_class_name,
                      mtat_access,
                      mtat_name,
                      mtat_type,
                      mtat_length,
                      mtat_operator,
                      mtat_title,
                      mtat_displen,
                      mtat_browse_order,
                      mtat_view_order,
                      mtat_input_method,
                      mtat_default,
                      mtat_htmltagatt)
                  values(
                      classname,
                      '31',
                      mtat_row.attname,
                      mtat_row.typname,
                      mtat_row.mtat_length,
                      '~*',
                      trim(both from mtat_row.att_title),
                      mtat_row.mtat_length,
                      mtat_row.attnum * 10,
                      mtat_row.attnum * 10,
                      mtat_row.mtat_input_method,
                      '',
                      '');
          x_counter := x_counter + 1;
        END LOOP;
        raise notice 'additions = %', x_counter;

        -- cleanup obsolete attributes
        delete from mtat
        where  mtat_class_name = classname
        and    mtat_input_method !~* '.*virtual=.*'
        and    mtat_name not in
               (select attname
                from   pg_attribute
                where  attrelid = pg_class_oid);
        GET DIAGNOSTICS x_counter =  ROW_COUNT;
        raise notice 'deletions = %', x_counter;
--
    return true;
    END;
$$
LANGUAGE 'plpgsql';





