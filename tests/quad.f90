module testq
    use iso_fortran_env, only: output_unit, real128
    ! real number precision options: single, double, quad
    integer, parameter :: sp = selected_real_kind(p=5)
    integer, parameter :: dp = selected_real_kind(p=15)
    integer, parameter :: qp = selected_real_kind(p=30)
      

    contains

    subroutine single_quad(x)
        real(qp) :: x

        write(output_unit,*) x

    end subroutine single_quad

    real(qp) function return_quad()
        return_quad = 3.14_qp
    end function return_quad


    subroutine single_quadc(x)
        complex(qp) :: x

        write(output_unit,*) x

    end subroutine single_quadc

end module testq