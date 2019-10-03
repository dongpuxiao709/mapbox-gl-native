#pragma once

#include <mbgl/util/optional.hpp>
#include <mbgl/util/string.hpp>

#include <mapbox/value.hpp>

namespace mbgl {

using Value = mapbox::base::Value;
using NullValue = mapbox::base::NullValue;
using PropertyMap = mapbox::base::ValueObject;
using FeatureIdentifier = mapbox::feature::identifier;
using FeatureGeoJSON = mapbox::feature::feature<double>;
using FeatureState = mapbox::base::ValueObject;
using FeatureStates = std::unordered_map<std::string, FeatureState>;       // <featureID, FeatureState>
using LayerFeatureStates = std::unordered_map<std::string, FeatureStates>; // <sourceLayer, FeatureStates>

struct feature_mbgl : FeatureGeoJSON {
    std::string source;
    std::string sourceLayer;
    PropertyMap state;

    using GeometryType = mapbox::geometry::geometry<double>;

    feature_mbgl(const FeatureGeoJSON& f) : FeatureGeoJSON(f), source(), sourceLayer(), state() {}

    feature_mbgl() : FeatureGeoJSON(), source(), sourceLayer(), state() {}
    feature_mbgl(const GeometryType& geom_) : FeatureGeoJSON(geom_), source(), sourceLayer(), state() {}
    feature_mbgl(GeometryType&& geom_) : FeatureGeoJSON(std::move(geom_)), source(), sourceLayer(), state() {}
    feature_mbgl(const GeometryType& geom_, const PropertyMap& prop_)
        : FeatureGeoJSON(geom_, prop_), source(), sourceLayer(), state() {}
    feature_mbgl(GeometryType&& geom_, PropertyMap&& prop_)
        : FeatureGeoJSON(std::move(geom_), std::move(prop_)), source(), sourceLayer(), state() {}
    feature_mbgl(const GeometryType& geom_, const PropertyMap& prop_, const FeatureIdentifier& id_)
        : FeatureGeoJSON(geom_, prop_, id_), source(), sourceLayer(), state() {}
    feature_mbgl(const GeometryType&& geom_, PropertyMap&& prop_, FeatureIdentifier&& id_)
        : FeatureGeoJSON(std::move(geom_), std::move(prop_), std::move(id_)), source(), sourceLayer(), state() {}
};

using Feature = feature_mbgl;

template <class T>
optional<T> numericValue(const Value& value) {
    return value.match(
        [] (uint64_t t) {
            return optional<T>(t);
        },
        [] (int64_t t) {
            return optional<T>(t);
        },
        [] (double t) {
            return optional<T>(t);
        },
        [] (auto) {
            return optional<T>();
        });
}

inline optional<std::string> featureIDtoString(const FeatureIdentifier& id) {
    if (id.is<NullValue>()) {
        return nullopt;
    }

    return id.match(
        [](const std::string& value_) { return value_; }, [](uint64_t value_) { return util::toString(value_); },
        [](int64_t value_) { return util::toString(value_); }, [](double value_) { return util::toString(value_); },
        [](const auto&) -> optional<std::string> { return nullopt; });
}

} // namespace mbgl
