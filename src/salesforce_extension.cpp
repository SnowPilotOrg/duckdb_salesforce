#define DUCKDB_EXTENSION_MAIN

#include "salesforce_extension.hpp"
#include "duckdb.hpp"
#include "duckdb/common/exception.hpp"
#include "duckdb/common/string_util.hpp"
#include "duckdb/function/scalar_function.hpp"
#include "duckdb/main/extension_util.hpp"
#include <duckdb/parser/parsed_data/create_scalar_function_info.hpp>

// OpenSSL linked through vcpkg
#include <openssl/opensslv.h>

namespace duckdb {

inline void SalesforceScalarFun(DataChunk &args, ExpressionState &state, Vector &result) {
    auto &name_vector = args.data[0];
    UnaryExecutor::Execute<string_t, string_t>(
	    name_vector, result, args.size(),
	    [&](string_t name) {
			return StringVector::AddString(result, "Salesforce "+name.GetString()+" 🐥");;
        });
}

inline void SalesforceOpenSSLVersionScalarFun(DataChunk &args, ExpressionState &state, Vector &result) {
    auto &name_vector = args.data[0];
    UnaryExecutor::Execute<string_t, string_t>(
	    name_vector, result, args.size(),
	    [&](string_t name) {
			return StringVector::AddString(result, "Salesforce " + name.GetString() +
                                                     ", my linked OpenSSL version is " +
                                                     OPENSSL_VERSION_TEXT );;
        });
}

static void LoadInternal(DatabaseInstance &instance) {
    // Register a scalar function
    auto salesforce_scalar_function = ScalarFunction("salesforce", {LogicalType::VARCHAR}, LogicalType::VARCHAR, SalesforceScalarFun);
    ExtensionUtil::RegisterFunction(instance, salesforce_scalar_function);

    // Register another scalar function
    auto salesforce_openssl_version_scalar_function = ScalarFunction("salesforce_openssl_version", {LogicalType::VARCHAR},
                                                LogicalType::VARCHAR, SalesforceOpenSSLVersionScalarFun);
    ExtensionUtil::RegisterFunction(instance, salesforce_openssl_version_scalar_function);
}

void SalesforceExtension::Load(DuckDB &db) {
	LoadInternal(*db.instance);
}
std::string SalesforceExtension::Name() {
	return "salesforce";
}

std::string SalesforceExtension::Version() const {
#ifdef EXT_VERSION_SALESFORCE
	return EXT_VERSION_SALESFORCE;
#else
	return "";
#endif
}

} // namespace duckdb

extern "C" {

DUCKDB_EXTENSION_API void salesforce_init(duckdb::DatabaseInstance &db) {
    duckdb::DuckDB db_wrapper(db);
    db_wrapper.LoadExtension<duckdb::SalesforceExtension>();
}

DUCKDB_EXTENSION_API const char *salesforce_version() {
	return duckdb::DuckDB::LibraryVersion();
}
}

#ifndef DUCKDB_EXTENSION_MAIN
#error DUCKDB_EXTENSION_MAIN not defined
#endif
