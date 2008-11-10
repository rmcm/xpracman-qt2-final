--     $Id$
-- 
--   Copyright 2004 X=X Computer Software Trust
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



-- add an attribute - amount outstanding, updated via a trigger from the INVC table.

alter table patn add column patn_amount_outstanding numeric(12,2);
alter table dbtr add column dbtr_amount_outstanding numeric(12,2);

-- ------------------------------------------------------------
-- Update patient "amount outstanding"
-- ------------------------------------------------------------

create or replace function fn_amountoutstanding()
returns trigger
as 'DECLARE
        x_patn__sequence patn.patn__sequence%TYPE;
        x_dbtr_code dbtr.dbtr_code%TYPE;

    BEGIN
        
        if (TG_OP = ''INSERT'' ) then
          if (  new.invc_amount != 0.00::numeric and 
                new.invc_gst_amount != 0.00::numeric and
                new.invc_paid_amount != 0.00::numeric and
                new.invc_paid_gst_amount::numeric != 0.00::numeric ) then
            return new;
          end if;
          x_patn__sequence = new.invc_patn__sequence;
          x_dbtr_code = new.invc_dbtr_code;          
        end if;

        if (TG_OP = ''UPDATE'' ) then
          if(   new.invc_amount = old.invc_amount and
                new.invc_gst_amount = old.invc_gst_amount and
                new.invc_paid_amount = old.invc_paid_amount and
                new.invc_paid_gst_amount = old.invc_paid_gst_amount ) then
            return new;
          end if;
          x_patn__sequence = new.invc_patn__sequence;
          x_dbtr_code = new.invc_dbtr_code;
        end if;

        if (TG_OP = ''DELETE'' ) then
          x_patn__sequence = old.invc_patn__sequence;
          x_dbtr_code = old.invc_dbtr_code;
        end if;

        UPDATE  patn
        set     patn_amount_outstanding = (
                        select  sum(invc_balance(invc__sequence))
                        from    invc
                        where   invc_patn__sequence = x_patn__sequence )
        where   patn__sequence = x_patn__sequence;
        -- RAISE NOTICE ''setting patn_amount_outstanding for %'', x_patn__sequence;

        UPDATE  dbtr
        set     dbtr_amount_outstanding = (
                        select  sum(invc_balance(invc__sequence))
                        from    invc
                        where   invc_dbtr_code = x_dbtr_code )
        where   dbtr_code = x_dbtr_code;
        -- RAISE NOTICE ''setting dbtr_amount_outstanding for %'', x_dbtr_code;

    if (TG_OP = ''INSERT'' or TG_OP = ''UPDATE'' ) then
      return new;
    else
      return old;
    end if;

    END;'
  LANGUAGE 'plpgsql';

drop trigger tr_amountoutstanding on invc;
create trigger tr_amountoutstanding after insert or update or delete
    on invc for each row
    execute procedure fn_amountoutstanding();


-- add metadata.
delete from mtal where mtal_mtat_name = 'patn_amount_outstanding';
delete from mtat where mtat_name = 'patn_amount_outstanding';
insert into mtat(mtat_name, mtat_type, mtat_title, mtat_class_name, mtat_displen, mtat_browse_order, mtat_view_order, mtat_input_method) values('patn_amount_outstanding', 'numeric(12,2)', 'Amount Outstanding', 'patn', 30, 370, 370, 'RO');
insert into mtal(mtal_mtat_name,mtal_mtop_code,mtal_value,mtal_colour, mtal_priority)
        values('patn_amount_outstanding',4, '0.00','#FF0000', 1);

-- update history;
\set mttb_name '\'patn\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;

