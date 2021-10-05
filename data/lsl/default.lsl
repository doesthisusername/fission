{
    "components": [
        {
            "Title": {
                "background": {
                    "Vertical": [
                        [
                            1,
                            1,
                            1,
                            0.13
                        ],
                        [
                            1,
                            1,
                            1,
                            0
                        ]
                    ]
                },
                "text_color": null,
                "show_game_name": true,
                "show_category_name": true,
                "show_finished_runs_count": false,
                "show_attempt_count": true,
                "text_alignment": "Auto",
                "display_as_single_line": false,
                "display_game_icon": true,
                "show_region": false,
                "show_platform": false,
                "show_variables": true
            }
        },
        {
            "Splits": {
                "background": {
                    "Alternating": [
                        [
                            0,
                            0,
                            0,
                            0
                        ],
                        [
                            1,
                            1,
                            1,
                            0.04
                        ]
                    ]
                },
                "visual_split_count": 4,
                "split_preview_count": 1,
                "show_thin_separators": true,
                "separator_last_split": true,
                "always_show_last_split": true,
                "fill_with_blank_space": true,
                "display_two_rows": false,
                "current_split_gradient": {
                    "Vertical": [
                        [
                            0.2,
                            0.4509804,
                            0.95686275,
                            1
                        ],
                        [
                            0.08235294,
                            0.20784314,
                            0.45490196,
                            1
                        ]
                    ]
                },
                "split_time_accuracy": "Hundredths",
                "segment_time_accuracy": "Hundredths",
                "delta_time_accuracy": "Hundredths",
                "delta_drop_decimals": true,
                "show_column_labels": false,
                "columns": [
                    {
                        "name": "Time",
                        "start_with": "ComparisonTime",
                        "update_with": "SplitTime",
                        "update_trigger": "OnEndingSegment",
                        "comparison_override": null,
                        "timing_method": null
                    },
                    {
                        "name": "+/−",
                        "start_with": "Empty",
                        "update_with": "Delta",
                        "update_trigger": "Contextual",
                        "comparison_override": null,
                        "timing_method": null
                    }
                ]
            }
        },
        {
            "Timer": {
                "background": "Transparent",
                "timing_method": null,
                "height": 60,
                "color_override": null,
                "show_gradient": true,
                "digits_format": "SingleDigitSeconds",
                "accuracy": "Hundredths",
                "is_segment_timer": false
            }
        },
        {
            "PreviousSegment": {
                "background": {
                    "Vertical": [
                        [
                            1,
                            1,
                            1,
                            0.06
                        ],
                        [
                            1,
                            1,
                            1,
                            0.005
                        ]
                    ]
                },
                "comparison_override": null,
                "display_two_rows": false,
                "label_color": null,
                "drop_decimals": true,
                "accuracy": "Tenths",
                "show_possible_time_save": false
            }
        },
        {
            "TotalPlaytime": {
                "background": {
                    "Vertical": [
                        [
                            1,
                            1,
                            1,
                            0.06
                        ],
                        [
                            1,
                            1,
                            1,
                            0.005
                        ]
                    ]
                },
                "display_two_rows": false,
                "show_days": true,
                "label_color": null,
                "value_color": null
            }
        },
        {
            "Text": {
                "background": {
                    "Vertical": [
                        [
                            1,
                            1,
                            1,
                            0.06
                        ],
                        [
                            1,
                            1,
                            1,
                            0.005
                        ]
                    ]
                },
                "display_two_rows": false,
                "left_center_color": null,
                "right_color": null,
                "text": {
                    "Split": [
                        "Lorem ipsum",
                        "lol"
                    ]
                }
            }
        }
    ],
    "general": {
        "direction": "Vertical",
        "background": {
            "Plain": [
                0.06,
                0.06,
                0.06,
                1
            ]
        },
        "best_segment_color": [
            1,
            0.8333333,
            0,
            1
        ],
        "ahead_gaining_time_color": [
            0,
            0.8,
            0.21333332,
            1
        ],
        "ahead_losing_time_color": [
            0.38000003,
            0.82000005,
            0.49733335,
            1
        ],
        "behind_gaining_time_color": [
            0.82000005,
            0.38000003,
            0.38000003,
            1
        ],
        "behind_losing_time_color": [
            0.8,
            0,
            0,
            1
        ],
        "not_running_color": [
            0.67,
            0.67,
            0.67,
            1
        ],
        "personal_best_color": [
            0.08000004,
            0.64733326,
            1,
            1
        ],
        "paused_color": [
            0.48,
            0.48,
            0.48,
            1
        ],
        "thin_separators_color": [
            1,
            1,
            1,
            0.09
        ],
        "separators_color": [
            1,
            1,
            1,
            0.35
        ],
        "text_color": [
            1,
            1,
            1,
            1
        ]
    }
}