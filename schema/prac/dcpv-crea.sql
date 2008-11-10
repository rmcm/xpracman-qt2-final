--
--   Copyright 2008 X=X Computer Software Trust
--                   Kangaroo Ground Australia 3097
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



-- Create VIew "dcpv" based on "docs"
--

-- \i schema/DBNAME/dcpv-functions.sql   -- Functions need to be defined before the VIEW

create or replace view dcpv                         -- Document Printing View
  as select
 patn_psnam || ', '
            || coalesce(patn_fsnam, '?')
            || ' (DOB: '
            || coalesce(date(patn_dob)::text, '?')
            || ')'
                                        as dcpv_patient_name,
 docs_title                             as dcpv_title,
 coalesce((select date_trunc('second', max(dcpl__timestamp))::text
           from   dcpl
           where  dcpl_docs__sequence = docs__sequence
           and    dcpl_access_type = 'Printed'), 'not printed')
                                        as dcpv_last_printed,
--
-- standard headers
--
 docs__sequence                         as dcpv__sequence,              -- Row-ID
 docs__timestamp                        as dcpv__timestamp,             -- Timestamp
 docs__user_entry                       as dcpv__user_entry,            -- Entered by
 docs__status                           as dcpv__status                 -- Record status
 --
 from (docs LEFT join patn on (docs.docs_patn__sequence = patn.patn__sequence))

;

-- ------------------------------------------------------------
-- PERMISSIONS
-- ------------------------------------------------------------
grant select,insert,update on dcpv to public;

-- -- ------------------------------------------------------------
-- -- FUNCTIONS
-- -- ------------------------------------------------------------
-- --
-- -- Create a function to insert via view
-- -- This function returns the serial_number of the new "virtual" row.
-- -- This is used instead of an "insert" rule, as the latter cannot use "lastoid"
-- -- to recover the inserted row.
-- -- NOTE:: Although docs is listed here as the 'base' table, dcpl is the target table for
-- --        inserts.
--
-- -- ------------------------------------------------------------
-- -- Insert a row with supplied values
-- -- ------------------------------------------------------------
-- create or replace function dcpv_insert_row(a_dcpv dcpv)
-- returns integer as $$
--     DECLARE
--       rec_dcpv dcpv%ROWTYPE;
--       x_update text := '';
--       sep char(1) := '';
--
--     BEGIN
--       -- add the dcpv record
--       select dcpv_insert_row()
--         into rec_dcpv.dcpv__sequence;
--
--       -- change to supplied values
--       -- (only docs vars)
--
--       if (a_dcpv.dcpv_XX is not null) then
--         x_update := sep || 'docs_XX = \'' || a_dcpv.dcpv_XX || '\'::timestamp';
--         sep = ',';
--       end if;
--
--       if (x_update != '') then
--         x_update = 'update docs set ' || x_update || ' where docs__sequence = ' || rec_dcpv.dcpv__sequence;
--         EXECUTE x_update;
--       end if;
--
--       return rec_dcpv.dcpv__sequence;
--     END;
-- $$
-- LANGUAGE 'plpgsql';
-- --
-- -- ------------------------------------------------------------
-- -- Insert a row and return the PK sequence id (used by Marvin)
-- -- ------------------------------------------------------------
-- create or replace function dcpv_insert_row()
-- returns integer as $$
--     DECLARE
--       rec_docs docs%ROWTYPE;
--
--     BEGIN
--
--       -- add the docs record
--       select docs_insert_row()
--         into rec_docs.docs__sequence;
--
--       return rec_docs.docs__sequence;
--     END;
-- $$
-- LANGUAGE 'plpgsql';
-- --
--
-- ------------------------------------------------------------
-- Insert a row and return the PK sequence id (used by Marvin)
-- ------------------------------------------------------------
create or replace function dcpv_update_row(a_new dcpv, a_old dcpv)
returns integer as $$
    DECLARE
      rec_docs docs%ROWTYPE;

    BEGIN

      -- insert a new row into dcpl (docs print log)

      if (a_new.dcpv_last_printed is not null
          and a_new.dcpv_last_printed != a_old.dcpv_last_printed
                ) then
         insert into dcpl(dcpl_docs__sequence,
                         dcpl_access_type)
            values(a_new.dcpv__sequence,
                   'Printed');
         return 1;
      end if;

      return 0; --rec_dcpv.dcpv__sequence;
    END;
$$
LANGUAGE 'plpgsql';
--

-- ------------------------------------------------------------
-- RULES
-- ------------------------------------------------------------

-- Insert Rule
-- The insert is more flexibly handled by a function;
--    eg dcpv_insert_row() in dcpv-functions.sql
--    create or replace rule rl_dcpv_insert as on insert to dcpv
--         do instead
--         (
--           select dcpv_insert_row(new.*);
--         );
--
-- -- Update rule
 create or replace rule rl_dcpv_update as on update
        to dcpv
        do instead
        (
          select dcpv_update_row(new.*, old.*);
        );
--
--  -- Delete rule
--  create or replace rule rl_dcpv_delete as on delete
--         to dcpv
--         do instead
--         (
--          delete from docs
--          where  docs__sequence  = old.dcpv__sequence
--         );
--
-- ------------------------------------------------------------
-- METADATA
-- ------------------------------------------------------------
-- add the default metadata
select create_metadata('dcpv');
update mtcl set mtcl_title = 'Document Printing View', mtcl_order_by = 'order by dcpv__sequence' where mtcl_name = 'dcpv';
--
-- /home/source/bin/makeDBSubstitute.sh module database table
-- update mtat set mtat_title = '_@ATT_TITLE@_', mtat_displen = _@ATT_DISPLEN@_, mtat_input_method = '_@ATT_INPUT_METHOD@_', mtat_view_order = _@ATT_VIEW_ORDER@_ where mtat_name = '_@ATT_NAME@_';

-- update history;
\set mttb_name '\'dcpv\''
\set mttb_rcs_header '\'$Id: my-macros.dm,v 1.37 2007/02/19 08:18:43 source Exp $\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;

