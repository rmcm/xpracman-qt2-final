--     $Id: fn_column_default.sql 2936 2006-07-17 10:35:17Z source $
-- 
--   Copyright 2006 X=X Computer Software Trust
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



-- ------------------------------------------------------------
-- Function: 
-- 
-- Get a default from PG system tables. It doesn't seem possible to use the
-- "columns" view, as column_default returns null for non-owners of table
-- ------------------------------------------------------------

create or replace function fn_column_default(text, text)
returns text as $$

  DECLARE
    a_table ALIAS FOR $1;
    a_column ALIAS FOR $2;
    x_default text;
    x_return text;

    BEGIN

      select    pg_get_expr(adbin,adrelid)
      into      x_default
      from      pg_class, pg_attribute, pg_attrdef
      where     pg_class.oid = pg_attribute.attrelid
      and       pg_attribute.attrelid = pg_attrdef.adrelid
      and       pg_attribute.attnum = pg_attrdef.adnum
      and       relname = a_table
      and       attname = a_column;

      if (not found or x_default is null) then
        return null;
      end if;

      EXECUTE 'select cast(' || x_default || ' as text)' INTO x_return;

      RETURN x_return;

    END;
$$  LANGUAGE 'plpgsql';

