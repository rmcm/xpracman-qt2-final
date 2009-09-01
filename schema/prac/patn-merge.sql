--     $Id$
--
--   Copyright 2006 X=X Computer Software Trust
--                Kangaroo Ground Australia 3097
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
-- Function: Merge patient records
-- ------------------------------------------------------------

create or replace function fn_patn_merge(integer, integer)
returns integer AS $$
  DECLARE
    a_from ALIAS for $1;
    a_to   ALIAS for $2;
    x_class text;
    x_record record;
    x_sql text;
    x_count integer;
    x_total integer := 0;

    -- NAME TABLE.ATTRIBUTE%TYPE;
    -- NAME TABLE%ROWTYPE;

    BEGIN

      FOR x_record IN
        select  mtat_class_name
        from    mtat
        where   mtat_name ~ '_patn__sequence'
        and     mtat_class_name not in (select viewname from pg_views)
        and     mtat_class_name not in ('pate')

      LOOP

        x_class := x_record.mtat_class_name;
        x_sql :=  'update ' || x_class ||
                 ' set    ' || x_class || '_patn__sequence = ' || a_to ||
                 ' where  ' || x_class || '_patn__sequence = ' || a_from;
        EXECUTE x_sql;
        GET DIAGNOSTICS x_count = ROW_COUNT;
        RAISE NOTICE '%::%', x_count, x_sql;
        x_total := x_total + x_count;
      END LOOP;

      -- fix debtor
      update    invc
      set       invc_dbtr_code = '/'||trim( both from to_char(a_to,'0000000'))
      where     invc_patn__sequence = a_to
      and       invc_dbtr_code = '/'||trim( both from to_char(a_from,'0000000'));

      GET DIAGNOSTICS x_count = ROW_COUNT;
      RAISE NOTICE '%::dbtr', x_count;
      x_total := x_total + x_count;

    return x_total;

    END;
$$
LANGUAGE 'plpgsql';

