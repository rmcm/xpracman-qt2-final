-- -*-sql-*-
--      $Id: create_links.sql 1057 2001-12-16 04:03:14Z source $ 
--
--    Copyright 2001 X=X Computer Software Trust 
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
-- 
--

-- ------------------------------------------------------------
-- PL functiona to create mtrl and mtfn entries, using system 
--   tables (via mtsv).
-- ------------------------------------------------------------

drop function create_link_parents(text);
create function create_link_parents(text)
returns text
as 'DECLARE
   arg_reln alias for $1;
   
   BEGIN
        insert into mtcl(mtcl_access,
                         mtcl_name)
          select  ''127'',
                  ''-''
          except
          select  ''127'',
                  mtcl_name
          from    mtcl;

        insert into mtat(mtat_access,
                         mtat_name,
                         mtat_title,
                         mtat_class_name,
                         mtat_input_method)
         (select  ''127'',
                  mtsv_key,
                  ''Composite Index'',
                  ''-'',
                  ''RO''
          from    mtsv
          where   mtsv_master_class = arg_reln
          UNION
          select  ''127'',
                  mtsv_join,
                  ''Composite Index'',
                  ''-'',
                  ''RO''
          from    mtsv
          where   mtsv_master_class = arg_reln)
          EXCEPT
          select  ''127'',
                  mtat_name,
                  ''Composite Index'',
                  ''-'',
                  ''RO''
          from    mtat;
          return '''';
    END;'
  LANGUAGE 'plpgsql';

-- ------------------------------------------------------------
drop function create_fk_links(text);
create function create_fk_links(text)
returns integer
as 'DECLARE
   arg_reln alias for $1;
   tmp_record record;
   tmp_count integer := 0;

    -- NAME TABLE.ATTRIBUTE%TYPE;
    -- NAME TABLE%ROWTYPE;

    BEGIN
        -- update mtcl and mtat first
        execute ''select create_link_parents(''|| quote_literal(arg_reln) || '')'';
        -- now update mtfn
        insert into mtfn(mtfn_title,
                         mtfn_master_class,
                         mtfn_key,
                         mtfn_other_class,
                         mtfn_join,
                         mtfn_view)
          select  mtsv_other_class as tmp_title,
                  mtsv_master_class as tmp_master_class,
                  mtsv_key as tmp_key,
                  mtsv_other_class as tmp_other_class,
                  mtsv_join as tmp_join,
                  mtsv_join as tmp_view
          from    mtsv
          where   mtsv_master_class = arg_reln
          except
          select  mtfn_other_class as tmp_title,
                  mtfn_master_class as tmp_master_class,
                  mtfn_key as tmp_key,
                  mtfn_other_class as tmp_other_class,
                  mtfn_join as tmp_join,
                  mtfn_join as tmp_view
          from    mtfn
          where   mtfn_master_class = arg_reln;

          GET DIAGNOSTICS tmp_count = ROW_COUNT;
    return tmp_count;
    END;'
  LANGUAGE 'plpgsql';

-- ------------------------------------------------------------
drop function create_rl_links(text);
create function create_rl_links(text)
returns integer
as 'DECLARE
   arg_reln alias for $1;
   tmp_record record;
   tmp_count integer := 0;

    -- NAME TABLE.ATTRIBUTE%TYPE;
    -- NAME TABLE%ROWTYPE;

    BEGIN
        -- update mtcl and mtat first
        execute ''select create_link_parents(''|| quote_literal(arg_reln) || '')'';

        insert into mtrl(mtrl_title,
                         mtrl_master_class,
                         mtrl_key,
                         mtrl_other_class,
                         mtrl_join)
          select  mtsv_master_class as tmp_title,
                  mtsv_other_class as tmp_master_class,
                  mtsv_join as tmp_key,
                  mtsv_master_class as tmp_other_class,
                  mtsv_key as tmp_join
          from    mtsv
          where   mtsv_other_class = arg_reln
          except
          select  mtrl_other_class as tmp_title,
                  mtrl_master_class as tmp_master_class,
                  mtrl_key as tmp_key,
                  mtrl_other_class as tmp_other_class,
                  mtrl_join as tmp_join
          from    mtrl
          where   mtrl_master_class = arg_reln;

          GET DIAGNOSTICS tmp_count = ROW_COUNT;
    return tmp_count;
    END;'
  LANGUAGE 'plpgsql';


