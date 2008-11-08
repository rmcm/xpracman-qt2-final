--     $Id: mtau-functions.sql 2872 2006-06-01 02:30:46Z source $
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

-- -- ----------------------------------------
-- -- Create a type to use in SETOF function
-- -- ----------------------------------------
-- DROP TYPE mtau_changed_type CASCADE;
-- CREATE TYPE mtau_changed_type AS (
--     mtau_attribute text,
--     mtau_before text,
--     mtau_after text,
--     mtau__sequence integer
-- );
-- 
-- create or replace function tfunc(integer) returns setof record as $$
-- DECLARE
--   r record;
-- BEGIN
--   for r in select mtau_attributes from mtau where mtau__sequence = $1
--   loop
--     return next r;
--   end loop;
--   return;
-- END
-- $$ LANGUAGE PLPGSQL IMMUTABLE;
-- 
-- -- ----------------------------------------
-- -- Return set of changed attributes
-- -- ----------------------------------------
-- 
-- CREATE or REPLACE FUNCTION mtau_changed()
--         RETURNS SETOF mtau_changed_type AS $$
-- 
--     DECLARE
-- 
--         x_mtau mtau%ROWTYPE;
--         i_count integer;
--         n_count integer := 0;
--         c_count integer;
--         x_changed mtau_changed_type;
--         x_delimiter text := '~';
-- 
--     BEGIN
-- 
--     RAISE EXCEPTION 'THIS FUNCTION IS NOT AVAILABLE YET';
--     return null;
-- 
--     FOR x_mtau IN 
--         select  *
--         from    mtau
-- 
--       LOOP
--   --      RAISE NOTICE '%', x_mtau.mtau__sequence;
--         i_count := 0;
--         n_count := length(x_mtau.mtau_attributes);
--         c_count := 0;
--         WHILE i_count < n_count LOOP
--             if (substr(x_mtau.mtau_attributes, i_count, 1) = '~') then
--               c_count := c_count + 1;
--               -- extract the data
--               x_changed.mtau_attribute := split_part(x_mtau.mtau_attributes, x_delimiter, c_count);
--               x_changed.mtau_before    := split_part(x_mtau.mtau_before, x_delimiter, c_count);
--               x_changed.mtau_after     := split_part(x_mtau.mtau_after, x_delimiter, c_count);
--               x_changed.mtau__sequence := x_mtau.mtau__sequence;
--               -- return changed attributes
--               if (x_changed.mtau_before != x_changed.mtau_after) then
--                 RETURN NEXT x_changed;
--               end if;
--             end if;
--             i_count := i_count + 1;
--         END LOOP;
--       END LOOP;
-- 
--     RETURN;
--     END;
-- $$ LANGUAGE 'plpgsql' STABLE ;
-- 
-- ----------------------------------------
-- Insert logging record
-- ----------------------------------------

create or replace function fn_mtau_insert(text, int, text, text, text,text)
        returns bool AS $$
  DECLARE
    a_table_name ALIAS FOR $1;
    a_row__sequence ALIAS FOR $2;
    a_operation ALIAS FOR $3;
    a_attributes ALIAS FOR $4;
    a_before ALIAS FOR $5;
    a_after ALIAS FOR $6;

  BEGIN
        if (a_before = a_after) then
          return false;
        end if;

  insert into mtau(mtau_table_name,
                   mtau_row_sequence,
                   mtau_operation,
                   mtau_attributes,
                   mtau_before,
                   mtau_after)
           select  a_table_name,
                   a_row__sequence,
                   a_operation,
                   a_attributes,
                   a_before,
                   a_after;

  RAISE NOTICE 'fn_mtau_insert:: %',$2;
  return true;

  END;

$$ LANGUAGE 'plpgsql';

