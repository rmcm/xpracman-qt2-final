--     $Id$
-- 
--   Copyright 2007 X=X Computer Software Trust
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
-- Bank Deposit Report

-- ----------------------------------------
-- Create a type to use in SETOF function
-- ----------------------------------------
DROP TYPE bank_deposit_type CASCADE;
CREATE TYPE bank_deposit_type AS (
    bkdr_list_name text,
    bkdr_tdtp_desc text,
    bkdr_date text,
    bkdr_patient text,
    bkdr_drawer text,
    bkdr_bank text,
    bkdr_branch text,
    bkdr_amount numeric(12,2),
    bkdr_tdtp_total numeric(12,2),
    bkdr_list_total numeric(12,2),
    bkdr_grand_total numeric(12,2),
    bkdr_bkdp__sequence integer,
    bkdr__sequence integer,
    bkdr__timestamp timestamp,
    bkdr__user_entry text,
    bkdr__status text
);

-- ----------------------------------------
-- Create a function returning a list (string)
-- of credits associated with a payment
-- ----------------------------------------
CREATE or REPLACE FUNCTION patient_name_list( integer )
       RETURNS text AS $$

    DECLARE
    a_paym__sequence ALIAS for $1;
    x_credit_record record;
    x_list text := '';
    x_sep text := '';
    x_counter integer := 0;
    BEGIN
      FOR x_credit_record IN
      SELECT coalesce( crep_patient_name, '') || ' ($' || crep_cred_amount + crep_cred_gst_amount || ')' 
             as patient_record
      FROM   crep
      WHERE  crep_paym__sequence = a_paym__sequence
      ORDER BY crep__sequence

    LOOP
        x_counter := x_counter + 1;
        x_list := x_list || x_sep || '[' || x_counter || '] ' || x_credit_record.patient_record;
        x_sep := '  ';
    END LOOP;

    RETURN x_list;

    END;
$$ LANGUAGE 'plpgsql';

-- ----------------------------------------
-- Create a function returning SETOF
-- bank_deposit_type for specified batch
-- and listing group.
-- ----------------------------------------
CREATE or REPLACE FUNCTION bank_deposit_report( integer, text )
        RETURNS SETOF bank_deposit_type AS $$

    DECLARE
    a_batch ALIAS for $1;
    a_list  ALIAS for $2;
    x_record bank_deposit_type;

    BEGIN

    FOR x_record IN
    SELECT       tdtp_list,
                 tdtp_desc,
                 to_char(paym_date_entry,'DD-MM-YYYY HH24:MI'),
                 patient_name_list(paym__sequence),
                 paym_drawer,
                 paym_bank,
                 paym_branch,
                 paym_amount,
                 (select sum(paym_amount)
                  from paym p LEFT JOIN tdtp t ON (p.paym_tdtp_code = t.tdtp_code)
                  where p.paym_bkdp__sequence = a_batch
                  and   p.paym_tdtp_code = paym.paym_tdtp_code),
                 (select sum(paym_amount)
                  from paym p LEFT JOIN tdtp ON(p.paym_tdtp_code = tdtp.tdtp_code)
                  where p.paym_bkdp__sequence = a_batch
                  and   tdtp.tdtp_list = a_list),
                 (select sum(paym_amount)
                  from paym p
                  where p.paym_bkdp__sequence = a_batch),
                 paym_bkdp__sequence,
                 paym__sequence,
                 paym__timestamp,
                 paym__user_entry,
                 paym__status
    FROM         paym LEFT JOIN tdtp ON (tdtp.tdtp_code = paym.paym_tdtp_code)
    WHERE        paym_bkdp__sequence = a_batch
    AND          tdtp_list = a_list
    AND          paym_amount > 0.00
    ORDER BY tdtp_list, paym_tdtp_code, paym_date_entry

    LOOP
        RETURN NEXT x_record;
    END LOOP;

    END;
$$ LANGUAGE 'plpgsql';

-- ----------------------------------------
-- Create a function returning SETOF
-- bank_deposit_type for specified batch
-- (a convience function of above)
-- ----------------------------------------
CREATE or REPLACE FUNCTION bank_deposit_report( integer )
        RETURNS SETOF bank_deposit_type AS $$

    DECLARE
    a_batch ALIAS for $1;
    x_list record;
    x_record bank_deposit_type;

    BEGIN

    FOR x_list IN
    SELECT     distinct( tdtp_list ) as v_list
    FROM       tdtp
    ORDER BY   tdtp_list

    LOOP
        FOR x_record IN
        SELECT       *
        FROM         bank_deposit_report( a_batch, x_list.v_list )
        
        LOOP
            RETURN NEXT x_record;
        END LOOP;
    END LOOP;

    END;
$$ LANGUAGE 'plpgsql';

-- ----------------------------------------
-- Create a function returning SETOF
-- bank_deposit_type for date range
-- and listing group.
-- ----------------------------------------
CREATE or REPLACE FUNCTION bank_deposit_report( date, text )
        RETURNS SETOF bank_deposit_type AS $$

    DECLARE
    a_date  ALIAS for $1;
    a_list  ALIAS for $2;
    x_record bank_deposit_type;

    BEGIN

    FOR x_record IN
    SELECT       tdtp_list,
                 tdtp_desc,
                 to_char(paym_date_entry,'DD-MM-YYYY HH24:MI'),
                 coalesce( (select crep_patient_name
                  from   crep
                  where  crep_paym__sequence = paym__sequence
                  limit  1), ''),
                 paym_drawer,
                 paym_bank,
                 paym_branch,
                 paym_amount,
                 (select sum(paym_amount)
                  from paym p LEFT JOIN tdtp t ON (p.paym_tdtp_code = t.tdtp_code)
                  where date(p.paym_date_entry) = a_date
                  and   p.paym_tdtp_code = paym.paym_tdtp_code),
                 (select sum(paym_amount)
                  from paym p LEFT JOIN tdtp ON(p.paym_tdtp_code = tdtp.tdtp_code)
                  where date(p.paym_date_entry) = a_date
                  and   tdtp.tdtp_list = a_list),
                 (select sum(paym_amount)
                  from paym p
                  where date(p.paym_date_entry) = a_date),
                 paym_bkdp__sequence,
                 paym__sequence,
                 paym__timestamp,
                 paym__user_entry,
                 paym__status
    FROM         paym LEFT JOIN tdtp ON (tdtp.tdtp_code = paym.paym_tdtp_code)
    WHERE        date(paym_date_entry) = a_date
    AND          tdtp_list = a_list
    AND          paym_amount > 0.00
    ORDER BY tdtp_list, paym_tdtp_code, paym_date_entry

    LOOP
        RETURN NEXT x_record;
    END LOOP;

    END;
$$ LANGUAGE 'plpgsql';

-- ----------------------------------------
-- Create a function returning SETOF
-- bank_deposit_type for specified date
-- (a convience function of above)
-- ----------------------------------------
CREATE or REPLACE FUNCTION bank_deposit_report( date )
        RETURNS SETOF bank_deposit_type AS $$

    DECLARE
    a_date ALIAS for $1;
    x_list record;
    x_record bank_deposit_type;

    BEGIN

    FOR x_list IN
    SELECT     distinct( tdtp_list ) as v_list
    FROM       tdtp
    ORDER BY   tdtp_list

    LOOP
        FOR x_record IN
        SELECT       *
        FROM         bank_deposit_report( a_date, x_list.v_list )
        
        LOOP
            RETURN NEXT x_record;
        END LOOP;
    END LOOP;

    END;
$$ LANGUAGE 'plpgsql';

-- ----------------------------------------
-- Create a VIEW returning the unbanked 
-- batch, to create the metadata entries
-- Directly call the SETOF functions to
-- load other (sub)batches
-- ----------------------------------------
CREATE or REPLACE VIEW bkdr AS
 SELECT *
 FROM bank_deposit_report(0)
;

grant select on bkdr to public;

delete from mtcl where mtcl_name = 'bkdr';
select create_metadata('bkdr');

-- ----------------------------------------
-- Tailor meta-data
-- ----------------------------------------
update mtcl set mtcl_title = 'Bank Reports', 
       mtcl_userlabel = '<B>Date:</B> %1<BR><B>Drawer:</B> %2<BR><B>Amount:</B> %3, bkdr_date, bkdr_drawer, bkdr_amount',
       mtcl_order_by = 'order by bkdr_list_name, bkdr_tdtp_desc, bkdr_date::timestamp'
       where mtcl_name = 'bkdr';

update mtat set mtat_input_method = 'RO' where mtat_class_name = 'bkdr';
update mtat set mtat_title = 'Tender Type' where mtat_name = 'bkdr_tdtp_desc';
update mtat set mtat_title = 'Tender Type Total' where mtat_name = 'bkdr_tdtp_total';
update mtat set mtat_title = 'Group Total' where mtat_name = 'bkdr_list_total';

-- update history;
\set mttb_name '\'bkdr\''
\set mttb_rcs_header '\'$Id:$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;


