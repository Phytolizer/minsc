#ifndef MINSC_CODE_ANALYSIS_DIAGNOSTIC_BAG_H_
#define MINSC_CODE_ANALYSIS_DIAGNOSTIC_BAG_H_

#include <buf/buf.h>
#include <stdbool.h>
#include <stddef.h>

#include "minsc/code_analysis/diagnostic.h"
#include "minsc/code_analysis/syntax/syntax_kind.h"
#include "minsc/code_analysis/text/text_span.h"
#include "minsc/runtime/object.h"
#include "str/str.h"

typedef struct DiagnosticBag DiagnosticBag;

DiagnosticBag* diagnostic_bag_new(void);
void diagnostic_bag_free(DiagnosticBag* bag);

typedef BUF(Diagnostic) DiagnosticBuf;

DiagnosticBuf diagnostic_bag_iter(const DiagnosticBag* bag);
bool diagnostic_bag_empty(const DiagnosticBag* bag);

void diagnostic_bag_report_invalid_int64(DiagnosticBag* bag, TextSpan span, str text);
void diagnostic_bag_report_bad_character(DiagnosticBag* bag, size_t position, char character);
void diagnostic_bag_report_unexpected_token(
    DiagnosticBag* bag,
    TextSpan span,
    SyntaxKind actual_kind,
    SyntaxKind expected_kind
);
void diagnostic_bag_report_undefined_unary_operator(
    DiagnosticBag* bag,
    TextSpan span,
    str operator_text,
    ObjectType operand_type
);
void diagnostic_bag_report_undefined_binary_operator(
    DiagnosticBag* bag,
    TextSpan span,
    str operator_text,
    ObjectType left_type,
    ObjectType right_type
);
void diagnostic_bag_report_undefined_name(DiagnosticBag* bag, TextSpan span, str name);

void diagnostic_bag_concat(DiagnosticBag* bag, DiagnosticBag* other);

#endif  // MINSC_CODE_ANALYSIS_DIAGNOSTIC_BAG_H_
