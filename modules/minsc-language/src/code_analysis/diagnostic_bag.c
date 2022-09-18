#include "minsc/code_analysis/diagnostic_bag.h"

#include <buf/buf.h>
#include <stdlib.h>

#include "minsc/code_analysis/diagnostic.h"
#include "minsc/runtime/object.h"
#include "minsc/support/minsc_assert.h"

struct DiagnosticBag {
    DiagnosticBuf diagnostics;
};

static void report(DiagnosticBag* bag, TextSpan span, str message);

DiagnosticBag* diagnostic_bag_new(void) {
    DiagnosticBag* bag = malloc(sizeof(DiagnosticBag));
    MINSC_ASSERT(bag != NULL);
    bag->diagnostics = (DiagnosticBuf)BUF_NEW;
    return bag;
}

void diagnostic_bag_free(DiagnosticBag* bag) {
    if (bag == NULL) {
        return;
    }
    for (uint64_t i = 0; i < bag->diagnostics.len; i++) {
        diagnostic_free(bag->diagnostics.ptr[i]);
    }
    BUF_FREE(bag->diagnostics);
    free(bag);
}

DiagnosticBuf diagnostic_bag_iter(const DiagnosticBag* bag) {
    return (DiagnosticBuf)BUF_REF(bag->diagnostics.ptr, bag->diagnostics.len);
}

bool diagnostic_bag_empty(const DiagnosticBag* bag) {
    return bag->diagnostics.len == 0;
}

void diagnostic_bag_report_invalid_int64(DiagnosticBag* bag, TextSpan span, str text) {
    str message = str_printf(
        "The number " str_fmt " isn't a valid " str_fmt ".",
        str_arg(text),
        str_arg(object_type_string(OBJECT_TYPE_INT64))
    );
    report(bag, span, message);
}

void diagnostic_bag_report_bad_character(DiagnosticBag* bag, size_t position, char character) {
    str message = str_printf("Bad character input: '%c'.", character);
    TextSpan span = {.start = position, .length = 1};
    report(bag, span, message);
}

void diagnostic_bag_report_unexpected_token(
    DiagnosticBag* bag,
    TextSpan span,
    SyntaxKind actual_kind,
    SyntaxKind expected_kind
) {
    str message = str_printf(
        "Unexpected token <" str_fmt ">, expected <" str_fmt ">.",
        str_arg(syntax_kind_string(actual_kind)),
        str_arg(syntax_kind_string(expected_kind))
    );
    report(bag, span, message);
}

void diagnostic_bag_report_undefined_unary_operator(
    DiagnosticBag* bag,
    TextSpan span,
    str operator_text,
    ObjectType operand_type
) {
    str message = str_printf(
        "Unary operator '" str_fmt "' is not defined for type " str_fmt ".",
        str_arg(operator_text),
        str_arg(object_type_string(operand_type))
    );
    report(bag, span, message);
}

void diagnostic_bag_report_undefined_binary_operator(
    DiagnosticBag* bag,
    TextSpan span,
    str operator_text,
    ObjectType left_type,
    ObjectType right_type
) {
    str message = str_printf(
        "Binary operator '" str_fmt "' is not defined for types " str_fmt " and " str_fmt ".",
        str_arg(operator_text),
        str_arg(object_type_string(left_type)),
        str_arg(object_type_string(right_type))
    );
    report(bag, span, message);
}

void diagnostic_bag_concat(DiagnosticBag* bag, DiagnosticBag* other) {
    BUF_CONCAT(&bag->diagnostics, other->diagnostics);
    BUF_FREE(other->diagnostics);
    free(other);
}

static void report(DiagnosticBag* bag, TextSpan span, str message) {
    Diagnostic diagnostic = {.span = span, .message = message};
    BUF_PUSH(&bag->diagnostics, diagnostic);
}
