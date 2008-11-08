-- -*-sql-*-
--      $Id: trigger_utils.sql 2750 2005-10-30 00:06:26Z source $ 
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
-- General functions associated with any database
--

-- ------------------------------------------------------------
-- trigger view
-- ------------------------------------------------------------

create or replace view tr_view as
  select
 relname                as relname,
 (case when tgisconstraint is false
    then tgname
  else
     byteaparam(pg_trigger.tgargs, 0)
  end)                  as tgname,
 (case when tgisconstraint is false
    then ''
  else
     byteaparam(pg_trigger.tgargs, 2) || '/' 
        || byteaparam(pg_trigger.tgargs, 1)
        || '/' ||
     (case when pg_trigger.tgtype = 9
        then 'D'
      when pg_trigger.tgtype = 17
        then 'U'
      when pg_trigger.tgtype = 21
        then 'O'
      else '?'
    end)      
  end)                  as constraint

 from   pg_class JOIN pg_trigger ON (pg_class.oid = pg_trigger.tgrelid)
 where  relname IN (select mtcl_name from mtcl)
 order by relname, tgname
;

-- ------------------------------------------------------------
-- disable triggers
-- ------------------------------------------------------------

create or replace function disable_triggers()
returns integer as $$
    DECLARE
       count integer;

    BEGIN

    EXECUTE ''
       UPDATE pg_class
       SET    reltriggers = 0
       WHERE  relname in (select mtcl_name from mtcl);'';
       GET DIAGNOSTICS count = ROW_COUNT;
       return count;
    END;
$$
LANGUAGE 'plpgsql';


-- ------------------------------------------------------------
-- enable triggers
-- ------------------------------------------------------------

create or replace function enable_triggers()
returns integer as $$
    DECLARE
       count integer;

    BEGIN

    EXECUTE ''
       UPDATE pg_class
       SET    reltriggers =  (SELECT count(*)
                              FROM   pg_trigger
                              where  pg_class.oid = tgrelid)
       WHERE  relname in (select mtcl_name from mtcl);'';
       GET DIAGNOSTICS count = ROW_COUNT;
       return count;
    END;
$$
LANGUAGE 'plpgsql';


-- ------------------------------------------------------------
-- count triggers
-- ------------------------------------------------------------
drop function count_triggers();
create function count_triggers()
returns text as $$
    DECLARE
       count integer;
       tmp_rec record;
       output text;

    BEGIN

    count := 0;
    output := '''';
    for tmp_rec in 
      EXECUTE ''
         SELECT rpad(relname,10) ||
                to_char( reltriggers::integer,99999999) ||
                to_char( count_triggers,99999999) as output
         FROM   pg_class,
                (SELECT tgrelid, count(*) as count_triggers
                 FROM pg_trigger
                 GROUP BY tgrelid) as realtriggers
         WHERE  pg_class.oid = realtriggers.tgrelid 
         AND   relname in (select mtcl_name from mtcl)
         ORDER BY relname;''

      LOOP
        output := output || ''\n'' || tmp_rec.output;
        count := count + 1;
      END LOOP;

    -- GET DIAGNOSTICS count = ROW_COUNT;
    return output;
    END;
$$
LANGUAGE 'plpgsql';

-- ------------------------------------------------------------
-- Disable triggers for a single class
-- ------------------------------------------------------------
create or replace function disable_triggers(text)
    returns integer as $$
  DECLARE
        a_class ALIAS for $1;
        x_count integer;

  BEGIN
        select  count_enabled_triggers(a_class)
        into    x_count;

        UPDATE  pg_class 
        SET     reltriggers = 0 
        WHERE   relname = a_class;

    return x_count;
  END;
$$
LANGUAGE 'plpgsql';

-- ------------------------------------------------------------
-- Enable triggers for a single class
-- ------------------------------------------------------------
create or replace function enable_triggers(text)
    returns integer as $$
  DECLARE
        a_class ALIAS for $1;
        x_count integer;

  BEGIN
        select  count_triggers(a_class)
        into    x_count;

        UPDATE  pg_class 
        SET     reltriggers = x_count
        WHERE   relname = a_class;

    return x_count;
  END;
$$
LANGUAGE 'plpgsql';

-- ------------------------------------------------------------
-- count triggers for a single class
-- ------------------------------------------------------------
create or replace function count_triggers(text)
    returns integer as $$
  DECLARE
        a_class ALIAS for $1;
        x_count integer;

  BEGIN
        select  count(*)
        into    x_count
        from    pg_trigger
        where   tgrelid = (select oid from pg_class where relname = a_class);

    return x_count;
  END;
$$
LANGUAGE 'plpgsql';

-- ------------------------------------------------------------
-- count triggers for a single class
-- ------------------------------------------------------------
create or replace function count_enabled_triggers(text)
    returns integer as $$
  DECLARE
        a_class ALIAS for $1;
        x_count integer;

  BEGIN
        select  reltriggers
        into    x_count
        from    pg_class
        where   relname = a_class;

    return x_count;
  END;
$$
LANGUAGE 'plpgsql';
